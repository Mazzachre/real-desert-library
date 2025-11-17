#pragma once

#include "person.h"

struct Cast {
    Person person;
    QString role;

    explicit Cast(const QJsonObject& obj);
    explicit Cast(const QSqlRecord& rec);

    bool operator<(const Cast& other) const;
};

QDebug operator<<(QDebug dbg, const Cast& data);
