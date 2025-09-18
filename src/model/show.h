#pragma once

#include <QString>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QSqlRecord>
#include "person.h"
#include "episode.h"

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

    QMap<quint16, QString> genres;
    QMap<quint16, QString> tags;
    QMap<QString, Person> cast;
    QMap<QString, Person> crew;

    explicit Show();
    explicit Show(const QJsonObject& data);
};

QDebug operator<<(QDebug dbg, const Show& data);
