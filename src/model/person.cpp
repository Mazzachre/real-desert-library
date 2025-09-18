#include "person.h"

Person::Person() {
}

Person::Person(const QJsonObject& data) {
    id = data[u"id"].toInt();
    name = data[u"name"].toString();
    originalName = data[u"original_name"].toString();
    profilePath = data[u"profile_path"].toString();
}

Person::Person(const QSqlRecord& record) {
    id = record.value("id").toInt();
    name = record.value("name").toString();
    originalName = record.value("original_name").toString();
    profilePath = record.value("profile_path").toString();
}

QDebug operator<<(QDebug dbg, const Person& data) {
    dbg.nospace().noquote() << "Person:(" << data.id << " " <<  data.name << ")";
    return dbg;
}
