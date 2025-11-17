#pragma once

#include <QString>
#include "../enums/extras-type.h"
#include "playback.h"

struct Extra {
    Rd::Enums::ExtrasType::Type type;
    quint32 id;
    QString name;
    quint32 fileId;
};

QDebug operator<<(QDebug dbg, const Extra& data);
