#include "extra.h"
#include <QDebug>

QDebug operator<<(QDebug dbg, const Extra& data) {
    dbg.nospace().noquote() << "Extra:(" << data.type << " " << data.id << " "<< data.name << " " << data.fileId << ")";
    return dbg;
}
