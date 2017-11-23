#pragma once

#include "Model.h"
#include "TaskProcessor.h"
#include <serialPortCom/ascii/FlightMetaInfo.h>

class DumpTask: public Task {
public:
    DumpTask( QString dir,
              FlightMetaInfo info,
              Model model );
    void run() override;

    FlightMetaInfo info() const;

private:
    QString dir_;
    FlightMetaInfo info_;
    Model model_;
};
