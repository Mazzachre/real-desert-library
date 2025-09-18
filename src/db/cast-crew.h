#pragma once

#include <QObject>
#include <QSqlError>
#include <QSqlDatabase>
#include "../model/person.h"

namespace Rd {
    namespace Database {
        class CastCrew : public QObject {
            Q_OBJECT
        public:
            explicit CastCrew(QObject* parent = nullptr);
            ~CastCrew() noexcept;

            QSqlError createPerson(const Person& person);
            QSqlError findPeople(const QString& name, QList<Person>& people);
        };
    }
}
