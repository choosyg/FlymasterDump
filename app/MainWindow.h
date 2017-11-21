#pragma once

#include "QtCallback.h"
#include "Model.h"

#include <serialPortCom/SerialPort.h>
#include <serialPortCom/ascii/FlightMetaInfo.h>

#include <QListWidgetItem>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>

#include <map>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = 0 );
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void on_saveButton_clicked();

    void onCallbackNotified( Severity s, QString m );

    void on_actionExit_triggered();

private:
    Ui::MainWindow* ui;
    Model model_;

    QtCallback cb_;
    std::map< QListWidgetItem*, FlightMetaInfo > itemInfo_;

    std::unique_ptr< SerialPort > openPort();
    void executeAsync( std::function<void(void)> f );
    void exportToIgc( SerialPort& port, QListWidgetItem* item, QString dir );

    void sync( Property<QString>& p, QLineEdit* edit );
};
