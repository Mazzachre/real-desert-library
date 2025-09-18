#pragma once

#include <QObject>
#include <QList>
#include <QUrl>
#include "../lib/media-meta.h"
#include "../db/files.h"

namespace Rd {
    namespace Ui {
        class FileHandler : public QObject {
            Q_OBJECT
        public:
            static FileHandler* instance();

            Q_INVOKABLE bool acceptDrag(const QList<QUrl>& urls);
            Q_INVOKABLE void filesDropped(const QList<QUrl>& urls);

            Q_SIGNAL void filesAdded(const QList<QVariantMap>& files);
            Q_SIGNAL void error(const QString& header, const QString& body);
            Q_SIGNAL void warn(const QString& text);
        private:
            explicit FileHandler(QObject* parent = nullptr);
            ~FileHandler() noexcept;
            Rd::Library::MediaMeta* m_mediaMeta;
            Rd::Database::Files* m_db;
        };
    }
}
