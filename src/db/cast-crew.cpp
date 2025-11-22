#include "cast-crew.h"
#include <QSqlQuery>

Rd::Database::CastCrew::CastCrew(QObject* parent)
: QObject(parent) {
}

Rd::Database::CastCrew::~CastCrew() noexcept {
}

QSqlError Rd::Database::CastCrew::createPerson(const Person& person) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO cast_crew (id, name, original_name, profile_path) VALUES (:id, :name, :original_name, :profile_path)");
    query.bindValue(":id", person.id);
    query.bindValue(":name", person.name);
    query.bindValue(":original_name", person.originalName);
    query.bindValue(":profile_path", person.profilePath);
    if (!query.exec()) {
        return query.lastError();
    }
    return QSqlError();
}

QSqlError Rd::Database::CastCrew::findPeople(const QString& name, QList<Person>& people) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM cast_crew WHERE LOWER(name) LIKE '%' || :name || '%' OR LOWER(original_name) LIKE '%' || :name || '%'");
    query.bindValue(":name", QVariant(name.toLower()));
    if (!query.exec()) {
        return query.lastError();
    }
    while(query.next()) {
        Person person(query.record());
        people.append(person);
    }

    return QSqlError();
}
