#pragma once

#include <QDebug>
#include <QMap>
#include <QList>
#include "cast.h"
#include "crew.h"
#include "playback.h"

struct EpisodeListItem {
    quint32 id;
    quint8 season;
    quint8 episode;
    QString name;
    QString overview;
    QDate airDate;
    quint16 officialRuntime;
    bool favorite;
    bool hasExtras;

    QList<Cast> cast;
    QList<Crew> crew;

    quint32 fileId;
    QString path;
    quint16 actualRuntime;
    QVariantMap meta;
    QVariantMap video;
    QVariantMap audio;
    QVariant selectedSubtitle;
    QVariant selectedVideo;
    QVariant selectedAudio;
    QVariantMap subtitles;
    bool multiple;

    QList<Playback> playback;

    bool isPlayable() const;
};

QDebug operator<<(QDebug dbg, const EpisodeListItem& data);
