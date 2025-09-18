#pragma once

#include <QString>
#include <QDate>

struct MovieSearchResult {
    quint32 id;
    QString title;
    QString originalLanguage;
    QString originalTitle;
    QString overview;
    double popularity;
    QString posterPath;
    QDate releaseDate;

    explicit MovieSearchResult();
    explicit MovieSearchResult(const QJsonObject& data);
};

QDebug operator <<(QDebug dbg, const MovieSearchResult& result);

