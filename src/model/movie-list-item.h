#pragma once

#include <QString>
#include <QDate>
#include <QSqlRecord>

struct MovieListItem {
    quint32 id;
    QString title;
    QString originalTitle;
    QDate releaseDate;

    explicit MovieListItem();
    explicit MovieListItem(const QSqlRecord& record);
};

QDebug operator<<(QDebug dbg, const MovieListItem& data);
