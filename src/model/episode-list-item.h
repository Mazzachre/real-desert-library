#pragma once

#include <QDebug>
#include <QMap>
#include "person.h"

struct EpisodeListItem {
    quint32 id;
    quint8 season;
    quint8 episode;
    QString name;
    QString overview;
    QDate airDate;
    quint16 officialRuntime;
    bool favorite;

    QMap<QString, Person> cast;
    QMap<QString, Person> crew;

    QString path;
    quint16 actualRuntime;
    QVariantMap meta;
    QVariantMap video;
    QVariantMap audio;
    QVariantMap subtitles;
    bool multiple;

    //TODO playbacks go in here too
};

QDebug operator<<(QDebug dbg, const EpisodeListItem& data);
