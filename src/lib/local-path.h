#pragma once

#include <QObject>
#include <QUrl>

namespace Rd {
    namespace Library {
        class LocalPath : public QObject {
            Q_OBJECT
        public:
            explicit LocalPath(QObject* parent = nullptr);
            ~LocalPath() noexcept;

            bool isLocalOrSmb(const QUrl& url);
            QString getLocalPath(const QUrl& url);
        private:
            QString findMountedKioFusePath(const QUrl& url);
            QString mountViaKioFuse(const QUrl& url);
        };
    }
}
