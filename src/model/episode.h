#pragma once

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QMap>
#include "cast.h"
#include "crew.h"

struct Episode {
    quint32 id;
    quint32 showId;
    quint8 season;
    quint8 episode;
    QString name;
    QString overview;
    QDate airDate;
    quint16 runtime;
    bool favorite;

    QList<Cast> cast;
    QList<Crew> crew;

    explicit Episode();
    explicit Episode(const QJsonObject& data);
};

QDebug operator<<(QDebug dbg, const Episode& data);
