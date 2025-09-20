#pragma once

#include <QObject>
#include <QFutureWatcher>
#include <QString>
#include <QtConcurrent>
#include <QSqlError>
#include "../model/file.h"
#include "local-path.h"

namespace Rd {
    namespace Library {
        class MediaMeta : public QObject {
            Q_OBJECT
        public:
            explicit MediaMeta(QObject* parent = nullptr);
            ~MediaMeta() noexcept;

            void mimeSort(const QList<QUrl>& urls);
            Q_SIGNAL void mimeSorted(const QList<QUrl>& videos, const QMap<QString, QUrl>& subtitles);

            void getFilesMeta(const QList<QUrl>& urls, const QMap<QString, QUrl>& subtitles);
            Q_SIGNAL void filesMeta(const QList<File>& files);

            Q_SIGNAL void error(const QString& header, const QString& body);
        private:
            Rd::Library::LocalPath* m_localPath;

            QVariantMap doMimeSort(const QList<QUrl>& urls);
            QFutureWatcher<QVariantMap> m_mimeWatcher;

            QList<File> doGetFilesMeta(const QList<QUrl>& urls, const QMap<QString, QUrl>& subtitles);
            QFutureWatcher<QList<File>> m_metaWatcher;

            File getFileMeta(const QUrl& url);
        };
    }
}
