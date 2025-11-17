#pragma once

#include <QObject>
#include <QUrl>
#include <QDir>

namespace Rd {
    namespace Library {
        class LocalPath : public QObject {
            Q_OBJECT
        public:
            explicit LocalPath(QObject* parent = nullptr);
            ~LocalPath() noexcept;

            bool isLocalOrSmb(const QUrl& url);
            QString getLocalPath(const QUrl& url);
            QUrl reverseLocalPath(const QString& url);
        private:
            QString m_base;

            QString findMountedKioFusePath(const QUrl& url);
            QString mountViaKioFuse(const QUrl& url);
        };
    }
}
