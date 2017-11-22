#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <serialPortCom/IgcFactory.h>
#include <serialPortCom/Request.h>
#include <serialPortCom/SerialPort.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QProgressDialog>
#include <QComboBox>

#include <fstream>
#include <future>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    ui->baudComboBox->addItem( "9600", static_cast< unsigned int >( Speed::BAUD9600 ) );
    ui->baudComboBox->addItem( "57600", static_cast< unsigned int >( Speed::BAUD57600 ) );
    ui->baudComboBox->addItem( "115200", static_cast< unsigned int >( Speed::BAUD115200 ) );
    ui->baudComboBox->setCurrentIndex( 2 );
    connectProperty( model_.serialPortSpeed, this, [&](){
        auto idx = ui->baudComboBox->findData( static_cast< unsigned int >( model_.serialPortSpeed() ) );
        ui->baudComboBox->setCurrentIndex( idx );
    });
    connect( ui->baudComboBox, &QComboBox::currentTextChanged, this, [&](){
         model_.serialPortSpeed = static_cast< Speed >( ui->baudComboBox->currentData().toInt() );
    });

    ui->comComboBox->addItem( "COM1", "COM1" );
    ui->comComboBox->addItem( "COM2", "COM2" );
    ui->comComboBox->addItem( "COM3", "COM3" );
    ui->comComboBox->addItem( "COM4", "COM4" );
    ui->comComboBox->addItem( "COM5", "COM5" );
    ui->comComboBox->setCurrentIndex( 2 );
    connectProperty( model_.serialPort, this, [&](){
        auto idx = ui->comComboBox->findData( model_.serialPort() );
        ui->comComboBox->setCurrentIndex( idx );
    });
    connect( ui->comComboBox, &QComboBox::currentTextChanged, this, [&](){
         model_.serialPort = ui->comComboBox->currentData().toString();
    });

    sync( model_.contest, ui->contestEdit );
    sync( model_.glider, ui->gliderEdit );
    sync( model_.gliderSerial, ui->gliderSerialEdit );
    sync( model_.pilotId, ui->pilotIdEdit );
    sync( model_.pilot, ui->pilotEdit );
    sync( model_.site, ui->siteEdit );

    load( model_ );
}

MainWindow::~MainWindow() {
    persist( model_ );
    delete ui;
}

std::unique_ptr< SerialPort > MainWindow::openPort() {
    std::unique_ptr< SerialPort > port( new SerialPort( model_.serialPort().toStdString() ) );
    port->setSpeed( model_.serialPortSpeed() );
    return port;
}

void MainWindow::executeAsync(std::function<void(QtCallback &)> f)
{
    QProgressDialog dlg( this );
    dlg.setRange(0,0);
    dlg.setMinimumWidth(400);
    QtCallback cb;
    QString lastInfo;
    connect( &dlg, &QProgressDialog::canceled, &cb, &QtCallback::cancel );
    connect( &cb, &QtCallback::notified, this, [&]( Severity s, const QString& msg ){
        if( s == Severity::Info ){
            lastInfo = msg;
            dlg.setLabelText( msg );
        } else if( s == Severity::Progress ){
            dlg.setLabelText( lastInfo + "\n" + msg );
        }
    });
    auto execute = [&](){
        try{
            f( cb );
        } catch( std::exception& e ){
            QMessageBox::critical( this, tr("Error"), tr("Unexpected error: %1").arg(e.what()) );
        }
        dlg.close();
    };
    auto future = std::async( std::launch::async, execute );
    dlg.exec();
    future.wait();
}

void MainWindow::on_connectButton_clicked() {
    try {
        auto port = openPort();

        Callback cb;
        auto flightInfos = requestFlightInfos( *port, cb );
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
                               tr("Unable to connect to %1 with %2 baud")
                               .arg( model_.serialPort() )
                               .arg( static_cast<unsigned int>( model_.serialPortSpeed() ) ) );
    }
}

void MainWindow::exportToIgc( SerialPort& port, QListWidgetItem* item, QString dir, QtCallback& cb ) {
    try{
        auto info = itemInfo_.at( item );
        auto bfd = requestFlight( port, info, cb );
        if( cb.count( Severity::Critical ) == 0  ){
            auto igc = buildIgc( info, bfd );
            igc.hRecord.pilot = model_.pilot().toStdString();
            igc.hRecord.gliderType = model_.glider().toStdString();
            igc.hRecord.gliderId = model_.gliderSerial().toStdString();
            igc.hRecord.contest = model_.contest().toStdString();
            igc.hRecord.site = model_.site().toStdString();

            std::string file = dir.toStdString() + "/" + info.date.year + "-" + info.date.month + "-" + info.date.day + ".igc";
            cb.notify( Severity::Info, "Storing IGC File: " + file );
            std::ofstream os( file );
            os << igc.build();
            os.flush();
            os.close();
        }
    }catch( std::exception& e){
        cb.notify( Severity::Critical, tr("Error: %1").arg( e.what() ).toStdString() );
    }

    if( cb.count( Severity::Critical ) == 0 ){
        item->setIcon( style()->standardIcon(QStyle::SP_DialogApplyButton) );
        item->setToolTip( "Successfully stored IGC file" );
        item->setCheckState( Qt::Unchecked );
        item->setFlags( item->flags() ^ Qt::ItemIsUserCheckable ^ Qt::ItemIsEnabled );
    } else {
        item->setIcon( style()->standardIcon(QStyle::SP_MessageBoxCritical) );
        item->setToolTip( cb.messages( Severity::Critical ).join("\n") );
    }
}

void MainWindow::on_saveButton_clicked() {
    auto dir = QFileDialog::getExistingDirectory( this, "Folder to save IGC files", model_.recentSaveDir() );
    if( dir.isEmpty() ) {
        return;
    }
    model_.recentSaveDir = dir;

    auto port = openPort();

    auto f = [&]( QtCallback& cb ){
        for( int i = 0; i < ui->listWidget->count(); ++i ) {
            auto item = ui->listWidget->item( i );
            if( item->checkState() == Qt::Checked ) {
                cb.reset();
                exportToIgc( *port, item, dir, cb );
            }
        }
    };
    executeAsync( f );
}

void MainWindow::sync(Property<QString> &p, QLineEdit *edit){
    connect( &p, &Property<QString>::propertyChanged, edit, [&, edit](){
        if( edit->text() != p() ){
            edit->setText( p() );
        }
    });
    connect( edit, &QLineEdit::textChanged, this, [&]( const QString& text ){
        p = text;
    });
}
