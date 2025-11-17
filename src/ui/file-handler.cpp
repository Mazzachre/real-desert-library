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
, m_localPath{new Rd::Library::LocalPath}
, m_db{new Rd::Database::Files} {
    connect(m_mediaMeta, &Rd::Library::MediaMeta::error, this, [this](const QString& header, const QString& body) {
        Q_EMIT error(header, body);
    }, Qt::QueuedConnection);
    connect(m_mediaMeta, &Rd::Library::MediaMeta::mimeSorted, this, &FileHandler::handleMimeSorted);
    connect(m_mediaMeta, &Rd::Library::MediaMeta::filesMeta, this, &FileHandler::handleFilesMeta);
}

Rd::Ui::FileHandler::~FileHandler() noexcept {
    m_db->deleteLater();
    m_localPath->deleteLater();
    m_mediaMeta->deleteLater();
}

Rd::Ui::FileHandler * Rd::Ui::FileHandler::instance() {
    static FileHandler instance;
    return &instance;
}

bool Rd::Ui::FileHandler::acceptDrag(const QList<QUrl>& urls) {
    if (!m_files.isEmpty()) {
        Q_EMIT warn("File handler busy");
        return false;
    }
    if (!m_localPath->isLocalOrSmb(urls[0])) {
        Q_EMIT warn("Can't read network file.");
        return false;
    }

    QMimeDatabase db;
    auto mode = ModeHandler::instance()->mode();
    if (mode == ModeHandler::Show || mode == ModeHandler::Extras) {
        for(const QUrl& url : urls) {
            if (m_localPath->isLocalOrSmb(url)) {
                QMimeType type = db.mimeTypeForUrl(url);
                if(type.name().startsWith(u"video/")) return true;
            }
        }
    }
    //If movies then we accept a single movie file?
    //If movie then we can have multiple parts...

    return false;
}

void Rd::Ui::FileHandler::filesDropped(const QList<QUrl>& urls) {
    if (!m_files.isEmpty()) {
        Q_EMIT warn("File handler busy");
        return;
    }

    BusyHandler::instance()->setBusy(true);
    m_mediaMeta->mimeSort(urls);
}

void Rd::Ui::FileHandler::handleMimeSorted(const QList<QUrl>& videos, const QMap<QString, QUrl>& subtitles) {
    QList<QUrl> create;

    for(const QUrl& url : videos) {
        quint32 exists;
        m_db->exists(url.toString(), exists);
        if (exists == 0) {
            create << url;
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
                m_files << file;
            } else {
                Q_EMIT warn(QString("Path %1 is linked to %2 \"%3\"").arg(url.toLocalFile()).arg(link["type"].toString()).arg(link["id"].toString()));
            }

        }
    }

    m_mediaMeta->getFilesMeta(create, subtitles);
}

void Rd::Ui::FileHandler::handleFilesMeta(const QList<File>& files) {
    if (!files.isEmpty()) {
        QSqlDatabase db = QSqlDatabase::database();

        if (!db.transaction()) {
            Q_EMIT error("Error adding files", db.lastError().text());
            return;
        }

        for(File file : files) {
            QSqlError err = m_db->create(file);
            if (err.type() != QSqlError::NoError) {
                Q_EMIT error("Error adding files", err.text());
                db.rollback();
                return;
            }
            m_files << file;
        }

        if (!db.commit()) {
            Q_EMIT error("Error adding files", db.lastError().text());
            db.rollback();
            return;
        }
    }

    BusyHandler::instance()->setBusy(false);
    Q_EMIT filesAdded(m_files);
    m_files.clear();
}
