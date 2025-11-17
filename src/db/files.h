#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QSqlError>
#include "../model/file.h"

namespace Rd {
    namespace Database {
        class Files : public QObject {
            Q_OBJECT
        public:
            explicit Files(QObject* parent = nullptr);
            ~Files() noexcept;

            QSqlError exists(const QString& path, quint32& id);
            QSqlError get(const quint32& id, File& file);
            QSqlError create(File& file);
            QSqlError getLinks(const quint32& id, QVariantMap& links);
            QSqlError setSubtitle(const quint32& id, const QString& subtitle);
        };
    }
}
