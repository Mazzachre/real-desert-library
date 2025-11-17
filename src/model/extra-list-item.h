#pragma once

#include <QString>
#include <QList>
#include <QSqlRecord>
#include "playback.h"

struct ExtraListItem {
    quint32 id;
    QString name;
    quint32 fileId;
    QString path;
    quint16 runtime;
    QList<Playback> playback;

    explicit ExtraListItem(const QSqlRecord& record);
};

QDebug operator<<(QDebug dbg, const ExtraListItem& data);
