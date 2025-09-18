#include "file-handler.h"
#include "mode-handler.h"
#include "busy-handler.h"
#include "../db/files.h"
#include <QVariant>
#include <QMimeData>
#include <QMimeDatabase>
#include <QSqlDatabase>

Rd::Ui::FileHandler::FileHandler(QObject* parent)
: QObject(parent)
, m_mediaMeta{new Rd::Library::MediaMeta}
, m_db{new Rd::Database::Files} {
}

Rd::Ui::FileHandler::~FileHandler() noexcept {
    m_db->deleteLater();
    m_mediaMeta->deleteLater();
}

Rd::Ui::FileHandler * Rd::Ui::FileHandler::instance() {
    static FileHandler instance;
    return &instance;
}

bool Rd::Ui::FileHandler::acceptDrag(const QList<QUrl>& urls) {
    if (!urls[0].isLocalFile()) {
        Q_EMIT warn("Can't read network file.");
        return false;
    }

    QMimeDatabase db;
    auto mode = ModeHandler::instance()->mode();
    if (mode == ModeHandler::Show) {

        for(const QUrl& url : urls) {
            if (url.isLocalFile()) {
                QMimeType type = db.mimeTypeForUrl(url);
                if(type.name().startsWith(u"video/")) {
                    return true;
                }
                if(type.inherits("application/x-subrip")) {
                    return true;
                }
            }
        }
    }
    //If movies then we accept a single movie file?
    //If movie then we can have multiple parts...

    return false;
}

void Rd::Ui::FileHandler::filesDropped(const QList<QUrl>& urls) {
    QMimeDatabase mime;
    QSqlDatabase db = QSqlDatabase::database();
    Rd::Database::Files files;
    QList<File> adding;
    QList<QVariantMap> fileList;
    BusyHandler::instance()->setBusy(true);

    for(const QUrl& url : urls) {
        if (url.isLocalFile()) {
            QMimeType type = mime.mimeTypeForUrl(url);

            if(type.name().startsWith(u"video/")) {
                quint32 exists;
                files.exists(url.toLocalFile(), exists);
                if (exists == 0) {
                    File file(url.toLocalFile());
                    QSqlError err = m_mediaMeta->getFileMeta(file);
                    if (err.type() != QSqlError::NoError) {
                        Q_EMIT error("Unable to add file", err.text());
                        continue;
                    }
                    adding << file;
                } else {
                    QVariantMap link;
                    QSqlError err = m_db->getLinks(exists, link);
                    if (err.type() != QSqlError::NoError) {
                        Q_EMIT error("Unable to fetch existing file", err.text());
                        continue;
                    }
                    if (link.empty()) {
                        File file;
                        err = m_db->get(exists, file);
                        if (err.type() != QSqlError::NoError) {
                            Q_EMIT error("Unable to fetch existing file", err.text());
                            continue;
                        }
                        fileList.append({{"type", "known"}, {"file", QVariant::fromValue(file)}});
                    } else {
                        Q_EMIT warn(QString("Path %1 is linked to %2 \"%3\"").arg(url.toLocalFile()).arg(link["type"].toString()).arg(link["id"].toString()));
                    }
                }
            } else if (type.inherits("application/x-subrip")) {
                //TODO add subtitle?
                fileList.append({{"type", "subtitle"}, {"path", url.toLocalFile()}});
            }
        }
    }

    if (!db.transaction()) {
        Q_EMIT error("Error adding files", db.lastError().text());
        return;
    }

    for(auto& file : adding) {
        QSqlError err = m_db->create(file);
        if (err.type() != QSqlError::NoError) {
            Q_EMIT error("Error adding files", err.text());
            db.rollback();
            return;
        }
        fileList.append({{"type", "created"}, {"file", QVariant::fromValue(file)}});
    }

    if (!db.commit()) {
        Q_EMIT error("Error adding files", db.lastError().text());
        db.rollback();
        return;
    }

    BusyHandler::instance()->setBusy(false);
    Q_EMIT filesAdded(fileList);
}
