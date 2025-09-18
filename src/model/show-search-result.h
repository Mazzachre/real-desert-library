#pragma once

#include <QString>
#include <QDate>

struct ShowSearchResult {
    quint32 id;
    QString name;
    QString originalLanguage;
    QString originalName;
    QString overview;
    double popularity;
    QString posterPath;
    QDate firstAirDate;

    explicit ShowSearchResult();
    explicit ShowSearchResult(const QJsonObject& data);
};

QDebug operator<<(QDebug dbg, const ShowSearchResult& data);
