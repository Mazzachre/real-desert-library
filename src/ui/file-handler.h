#pragma once

#include <QObject>
#include <QList>
#include <QUrl>
#include "lib/media-meta.h"
#include "lib/local-path.h"
#include "db/files.h"

namespace Rd {
    namespace Ui {
        class FileHandler : public QObject {
            Q_OBJECT
        public:
            static FileHandler* instance();

            Q_INVOKABLE bool acceptDrag(const QList<QUrl>& urls);
            Q_INVOKABLE void filesDropped(const QList<QUrl>& urls);

            Q_SIGNAL void filesAdded(const QList<File>& files);
            Q_SIGNAL void error(const QString& header, const QString& body);
            Q_SIGNAL void warn(const QString& text);
        private:
            explicit FileHandler(QObject* parent = nullptr);
            ~FileHandler() noexcept;
            Rd::Library::MediaMeta* m_mediaMeta;
            Rd::Library::LocalPath* m_localPath;
            Rd::Database::Files* m_db;
            QList<File> m_files;

            Q_SLOT void handleMimeSorted(const QList<QUrl>& videos, const QMap<QString, QUrl>& subtitles);
            Q_SLOT void handleFilesMeta(const QList<File>& files);
        };
    }
}
