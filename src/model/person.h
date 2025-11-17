#pragma once

#include <QString>
#include <QJsonObject>
#include <QSqlRecord>

struct Person {
    quint32 id;
    QString name;
    QString originalName;
    QString profilePath;

    explicit Person();
    explicit Person(const QJsonObject& data);
    explicit Person(const QSqlRecord& record);

    bool operator<(const Person& other) const;
};

QDebug operator<<(QDebug dbg, const Person& data);
