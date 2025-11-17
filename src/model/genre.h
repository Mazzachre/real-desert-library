#pragma once

#include <QString>
#include <QDebug>
#include <QSqlRecord>

struct Genre {
    quint32 id;
    QString name;
    QString source;

    Genre();
    explicit Genre(const QSqlRecord& record);

    bool operator==(const Genre& other) const;
    bool operator<(const Genre& other) const;
};

QDebug operator<<(QDebug dbg, const Genre& data);
