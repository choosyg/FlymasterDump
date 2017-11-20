#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "QtCallback.h"

#include <serialPortCom/IgcFactory.h>
#include <serialPortCom/Request.h>
#include <serialPortCom/SerialPort.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QProgressDialog>

#include <fstream>
#include <future>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );
    connect( &cb_, SIGNAL( notified( Severity, QString ) ), this, SLOT( onCallbackNotified( Severity, QString ) ) );

    ui->baudComboBox->addItem( "9600", static_cast< unsigned int >( Speed::BAUD9600 ) );
    ui->baudComboBox->addItem( "57600", static_cast< unsigned int >( Speed::BAUD57600 ) );
    ui->baudComboBox->addItem( "115200", static_cast< unsigned int >( Speed::BAUD115200 ) );
    ui->baudComboBox->setCurrentIndex( 2 );

    ui->comComboBox->addItem( "COM1" );
    ui->comComboBox->addItem( "COM2" );
    ui->comComboBox->addItem( "COM3" );
    ui->comComboBox->addItem( "COM4" );
    ui->comComboBox->addItem( "COM5" );
    ui->comComboBox->setCurrentIndex( 2 );

    loadUserInput();
}

MainWindow::~MainWindow() {
    delete ui;
}

std::unique_ptr< SerialPort > MainWindow::openPort() {
    std::unique_ptr< SerialPort > port( new SerialPort( ui->comComboBox->currentText().toStdString() ) );
    port->setSpeed( static_cast< Speed >( ui->baudComboBox->currentData().toInt() ) );
    return port;
}

void MainWindow::on_connectButton_clicked() {
    try {
        auto port = openPort();

        auto serial = requestSerial( *port, cb_ );
        ui->logBox->append( serial.c_str() );

        auto flightInfos = requestFlightInfos( *port, cb_ );
        ui->listWidget->clear();
        for( const auto& info : flightInfos ) {
            QString flight = QString( "%1: %2-%3-%4 %5:%6:%7 Duration: %8:%9:%10" )
                                 .arg( info.index )
                                 .arg( info.date.year.c_str() )
                                 .arg( info.date.month.c_str() )
                                 .arg( info.date.day.c_str() )
                                 .arg( info.time.hours.c_str() )
                                 .arg( info.time.minutes.c_str() )
                                 .arg( info.time.seconds.c_str() )
                                 .arg( info.duration.hours.c_str() )
                                 .arg( info.duration.minutes.c_str() )
                                 .arg( info.duration.seconds.c_str() );
            QListWidgetItem* item = new QListWidgetItem( flight, ui->listWidget );
            item->setFlags( item->flags() | Qt::ItemIsUserCheckable );
            item->setCheckState( Qt::Unchecked );
            itemInfo_.insert( std::make_pair( item, info ) );
        }
        ui->saveButton->setEnabled( true );
    } catch( std::exception& ) {
        QMessageBox::critical( this,
                               "Error",
                               "Unable to connect to " + ui->comComboBox->currentText() + " with "
                                   + ui->baudComboBox->currentText() + " baud" );
    }
}

void MainWindow::exportToIgc( SerialPort& port, QListWidgetItem* item, QString dir ) {
    auto info = itemInfo_.at( item );
    auto bfd = requestFlight( port, info, cb_ );
    auto igc = buildIgc( info, bfd );
    igc.hRecord.pilot = ui->pilotEdit->text().toStdString();
    igc.hRecord.gliderType = ui->gliderEdit->text().toStdString();
    igc.hRecord.gliderId = ui->gliderSerialEdit->text().toStdString();
    igc.hRecord.contest = ui->contestEdit->text().toStdString();
    igc.hRecord.site = ui->siteEdit->text().toStdString();

    std::string file = dir.toStdString() + "/" + info.date.year + "-" + info.date.month + "-" + info.date.day + ".igc";
    cb_.notify( Severity::Info, "Storing IGC File: " + file );
    std::ofstream os( file );
    os << igc.build();
    os.flush();
    os.close();
}

void MainWindow::saveUserInput() const {
    QSettings settings( "settings.ini", QSettings::IniFormat );
    settings.beginGroup( "Info" );
    settings.setValue( "Pilot", ui->pilotEdit->text() );
    settings.setValue( "PilotID", ui->pilotIdEdit->text() );
    settings.setValue( "Glider", ui->gliderEdit->text() );
    settings.setValue( "GliderSerial", ui->gliderSerialEdit->text() );
    settings.setValue( "Site", ui->siteEdit->text() );
    settings.setValue( "Contest", ui->contestEdit->text() );
    settings.endGroup();
}

void MainWindow::loadUserInput() {
    QSettings settings( "settings.ini", QSettings::IniFormat );
    settings.beginGroup( "Info" );
    ui->pilotEdit->setText( settings.value( "Pilot" ).toString() );
    ui->pilotIdEdit->setText( settings.value( "PilotID" ).toString() );
    ui->gliderEdit->setText( settings.value( "Glider" ).toString() );
    ui->gliderSerialEdit->setText( settings.value( "GliderSerial" ).toString() );
    ui->siteEdit->setText( settings.value( "Site" ).toString() );
    ui->contestEdit->setText( settings.value( "Contest" ).toString() );
    settings.endGroup();
}

void MainWindow::on_saveButton_clicked() {
    auto dir = QFileDialog::getExistingDirectory( this, "Folder to save IGC files" );
    if( dir.isEmpty() ) {
        return;
    }

    auto port = openPort();

    QProgressDialog dlg( this );
    dlg.setRange(0,0);
    dlg.setMinimumWidth(400);
    QString progressPrefix;
    cb_.setCancelled( false );
    connect( &dlg, &QProgressDialog::canceled, &cb_, &QtCallback::cancel );
    connect( &cb_, &QtCallback::notified, this, [&]( Severity s, const QString& msg ){
        if( s == Severity::Progress ){
            dlg.setLabelText( progressPrefix + msg );
        }
    });
    auto f = [&](){
        for( int i = 0; i < ui->listWidget->count(); ++i ) {
            auto item = ui->listWidget->item( i );
            if( item->checkState() == Qt::Checked ) {
                progressPrefix = "Reading " + item->text() +"\n";
                exportToIgc( *port, item, dir );
            }
        }
        dlg.close();
    };
    auto future = std::async( std::launch::async, f );
    dlg.exec();
    future.wait();
}

void MainWindow::onCallbackNotified( Severity s, QString m ) {
    switch( s ) {
        case Severity::Debug:
            return;
            m = "[DEBUG]   " + m;
            break;
        case Severity::Info:
            m = "[INFO]    " + m;
            break;
        case Severity::Warning:
            m = "[WARNING] " + m;
            break;
        case Severity::Error:
            m = "[ERROR]   " + m;
            break;
        case Severity::Critical:
            m = "[CRITICAL]" + m;
            break;
        default:
        return;
            break;
    }
    ui->logBox->append( m );
    ui->logBox->update();
}

void MainWindow::on_actionExit_triggered() {
    QApplication::quit();
}

void MainWindow::on_toolButton_clicked() {
    saveUserInput();
}
