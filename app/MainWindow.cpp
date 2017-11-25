#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "DumpTask.h"
#include "FlightListTask.h"

#include <serialPortCom/Request.h>
#include <serialPortCom/SerialPort.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QComboBox>


MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ), taskProgress_( processor_, this ) {
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
    sync( model_.gliderId, ui->gliderIdEdit );
    sync( model_.pilot, ui->pilotEdit );
    sync( model_.site, ui->siteEdit );

    load( model_ );

    connect( &processor_, &TaskProcessor::finishedTask, this, &MainWindow::onTaskFinished );
}

MainWindow::~MainWindow() {
    persist( model_ );
    delete ui;
}


void MainWindow::on_connectButton_clicked() {
    TaskPtr flt = std::make_shared<FlightListTask>( model_ );
    processor_.schedule( flt );
}

void MainWindow::on_saveButton_clicked() {
    auto dir = QFileDialog::getExistingDirectory( this, "Folder to save IGC files", model_.recentSaveDir() );
    if( dir.isEmpty() ) {
        return;
    }
    model_.recentSaveDir = dir;
    for( int i = 0; i < ui->listWidget->count(); ++i ) {
        auto item = ui->listWidget->item( i );
        if( item->checkState() == Qt::Checked ) {
            auto task = std::make_shared<DumpTask>( dir, itemInfo_.at( item ), model_ );
            processor_.schedule( task );
        }
    }
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

void MainWindow::onTaskFinished(TaskPtr task)
{
    DumpTask* d = dynamic_cast<DumpTask*>( task.get() );
    if( d != nullptr ){
        QListWidgetItem* item = 0;
        for( const auto& p : itemInfo_ ){
            if( p.second.index == d->info().index ){
                item = p.first;
            }
        }
        if( item == nullptr ){
            return;
        }
        if( task->callback().count( Severity::Critical ) == 0 && ! task->callback().isCancelled() ){
            item->setIcon( style()->standardIcon(QStyle::SP_DialogApplyButton) );
            item->setToolTip( "Successfully stored IGC file" );
            item->setCheckState( Qt::Unchecked );
            item->setFlags( item->flags() ^ Qt::ItemIsUserCheckable ^ Qt::ItemIsEnabled );
        } else {
            item->setIcon( style()->standardIcon(QStyle::SP_MessageBoxCritical) );
            item->setToolTip( task->callback().messages( Severity::Critical ).join("\n") );
        }
    }

    FlightListTask* flt = dynamic_cast<FlightListTask*>( task.get() );
    if( flt != nullptr ){
        if( task->callback().count( Severity::Critical ) == 0 ){
            ui->listWidget->clear();
            for( size_t i=0; i<flt->infos().size(); ++i ){
                QListWidgetItem* item = new QListWidgetItem( flt->list()[i], ui->listWidget );
                item->setFlags( item->flags() | Qt::ItemIsUserCheckable );
                item->setCheckState( Qt::Unchecked );
                itemInfo_.insert( std::make_pair( item, flt->infos()[i] ) );
            }
            ui->saveButton->setEnabled( true );
        } else {
            QMessageBox::critical( this,
                                   "Error",
                                   tr("Unable to connect to %1 with %2 baud")
                                   .arg( model_.serialPort() )
                                   .arg( static_cast<unsigned int>( model_.serialPortSpeed() ) ) );
        }
    }
}
