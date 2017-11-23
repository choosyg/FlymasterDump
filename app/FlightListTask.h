#pragma once
#include "Model.h"
#include "TaskProcessor.h"

#include <serialPortCom/ascii/FlightMetaInfo.h>

class FlightListTask : public Task {
public:
    FlightListTask( Model model );
    void run() override;

    const QStringList& list() const;

    const std::vector<FlightMetaInfo>& infos() const;

private:
    Model model_;
    QStringList list_;
    std::vector< FlightMetaInfo > infos_;
};
