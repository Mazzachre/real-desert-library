#pragma once

#include <QString>
#include <QDate>
#include <QSqlRecord>

struct ShowListItem {
    quint32 id;
    QString name;
    QString originalName;
    QDate firstAirDate;
    quint32 episodeCount;
    quint32 officialEpisodeCount;

    explicit ShowListItem();
    explicit ShowListItem(const QSqlRecord& record);
};

QDebug operator<<(QDebug dbg, const ShowListItem& data);
