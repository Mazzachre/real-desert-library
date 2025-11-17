#pragma once

#include <QString>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QSqlRecord>
#include "cast.h"
#include "crew.h"
#include "episode.h"
#include "genre.h"

struct Show {
    quint32 id;
    QString imdb;
    QString name;
    QString originalName;
    QStringList origin;
    QString tagline;
    QString overview;
    QStringList languages;
    QString posterPath;
    bool hasExtras;

    QList<Genre> genres;
    QVariantList tags;

    QList<Cast> cast;
    QList<Crew> crew;

    Show();
    explicit Show(const QJsonObject& data);
};

QDebug operator<<(QDebug dbg, const Show& data);
