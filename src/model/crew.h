#pragma once

#include "person.h"

struct Crew {
    Person person;
    QString job;

    explicit Crew(const QJsonObject& obj);
    explicit Crew(const QSqlRecord& rec);
};

QDebug operator<<(QDebug dbg, const Crew& data);
