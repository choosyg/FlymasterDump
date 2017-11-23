#pragma once

#include "TaskProcessor.h"

#include <QProgressDialog>

class TaskProgressDialog: public QProgressDialog {
public:
    TaskProgressDialog( TaskProcessor& processor, QWidget* parent = 0 );
private:
    QString lastInfo_;
};
