#pragma once

#include <QObject>
#include <QString>
#include "../model/file.h"
#include <QSqlError>

namespace Rd {
    namespace Library {
        class MediaMeta : public QObject {
            Q_OBJECT
        public:
            explicit MediaMeta(QObject* parent = nullptr);
            ~MediaMeta() noexcept;

            QSqlError getFileMeta(File& file);
        };
    }
}
