#include "local-path.h"
#include <unistd.h>
#include <QDBusInterface>
#include <QDBusReply>

Rd::Library::LocalPath::LocalPath(QObject* parent)
: QObject(parent) {
    m_base = "/run/user/" + QString::number(getuid()) + "/";
}

Rd::Library::LocalPath::~LocalPath() noexcept {
}

bool Rd::Library::LocalPath::isLocalOrSmb(const QUrl& url) {
    return url.isLocalFile() || url.scheme() == u"smb"_qs;
}

QString Rd::Library::LocalPath::getLocalPath(const QUrl& url) {
    if (url.isLocalFile()) {
        return url.toLocalFile();
    }

    QString existingPath = findMountedKioFusePath(url);
    if (!existingPath.isEmpty()) {
        return existingPath;
    }

    QString mountPath = mountViaKioFuse(url);
    if (!mountPath.isEmpty()) {
        return mountPath;
    }

    //OK we failed! fallback?

    return QString();
}

QUrl Rd::Library::LocalPath::reverseLocalPath(const QString& url) {
    if (url.startsWith(m_base + u"kio-fuse-"_qs)) {
        int index = url.indexOf(u"/smb/");
        if (index == -1) return QUrl();
        return QUrl(u"smb://"_qs + url.mid(index+5));
    }
    return QUrl::fromLocalFile(url);
}

QString Rd::Library::LocalPath::findMountedKioFusePath(const QUrl& url) {
    QDir dir(m_base);
    QStringList entries = dir.entryList(QStringList() << "kio-fuse*", QDir::Dirs);

    for (const QString &entry : entries) {
        QDir shareDir(QDir::cleanPath(m_base + u"/"_qs + entry + u"/"_qs + url.scheme() + u"/"_qs + url.host()));
        if (shareDir.exists(url.path().mid(1))) {
            return shareDir.absoluteFilePath(url.path().mid(1));
        }
    }

    return QString();
}

QString Rd::Library::LocalPath::mountViaKioFuse(const QUrl& url) {
    QDBusInterface iface(
        "org.kde.KIOFuse",
        "/org/kde/KIOFuse",
        "org.kde.KIOFuse.VFS",
        QDBusConnection::sessionBus());

    if (iface.isValid()) {
        QDBusReply<QString> reply = iface.call("mountUrl", url.toString());
        if (reply.isValid()) {
            if (QDir(reply.value()).exists(url.path().mid(1))) {
                return QDir(reply.value()).absoluteFilePath(url.path().mid(1));
            }
        }
    }

    return QString();
}
