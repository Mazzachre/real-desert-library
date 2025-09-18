#pragma once

#include <QObject>
#include <QSqlError>
#include "migration/migrate.h"

namespace Rd {
    namespace Database {
        class Database : public QObject {
            Q_OBJECT
        public:
            explicit Database(QObject* parent = nullptr);
            ~Database() noexcept;
            QSqlError open();
            QSqlError migrate();
            void close();
        };
    }
}
