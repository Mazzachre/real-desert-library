#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include "migration.h"
#include <vector>
#include <memory>

namespace Rd {
    namespace Database {
        namespace Migration {
            class Migrate : public QObject {
                Q_OBJECT
            public:
                explicit Migrate(QObject* parent = nullptr);
                ~Migrate() noexcept;
                QSqlError run();

            private:
                QSqlError init(const QSqlDatabase& db);
                QSqlError lastVersion(const QSqlDatabase& db, int& version);
                QSqlError update(const QSqlDatabase& db, int version);

                std::vector<std::unique_ptr<Migration>> m_migrations;
            };
        }
    }
}
