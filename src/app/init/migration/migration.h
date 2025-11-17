#pragma once

#include <QSqlDatabase>

namespace Rd {
    namespace Database {
        namespace Migration {
            class Migration {
            public:
                virtual ~Migration() = default;
                virtual int version() const = 0;
                virtual QSqlError apply(const QSqlDatabase& db) = 0;
            };
        }
    }
}
