#pragma once

#include <QObject>
#include <QSqlError>

namespace Rd {
    namespace Database {
        class Tags : public QObject {
            Q_OBJECT
        public:
            explicit Tags(QObject* parent = nullptr);
            ~Tags() noexcept;

            QSqlError getTags(QVariantList& tags);
            QSqlError addTag(const QString& tag, QVariantMap& added);
        };
    }
}
