#include "imdb.h"
#include "net/imdb/imdb.h"
#include "lib/imdb-parser.h"
#include <QThread>
#include <QSqlDatabase>
#include <QDateTime>

Rd::Application::Init::Imdb::Imdb(QObject* parent)
: QObject(parent) {
}

Rd::Application::Init::Imdb::~Imdb() noexcept {
}

void Rd::Application::Init::Imdb::start() {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM imdb_update LIMIT 1");
    if (!query.exec()) {
        Q_EMIT error(query.lastError().text());
        return;
    }
    if (query.next()) {
        Q_EMIT done();
        return;
    }

    m_createGenre = QSqlQuery(db);
    m_createLink = QSqlQuery(db);
    m_createGenre.prepare("INSERT INTO genres(name, source) VALUES(:name, 'IMDB')");
    m_createLink.prepare("INSERT INTO genre_links(imdb_id, genre_id, source) VALUES(:imdb, :genre, 'IMDB')");

    if (!db.transaction()) {
        Q_EMIT error(db.lastError().text());
        return;
    }

    auto net = new Net::Imdb::Imdb(this);
    auto parser = new Library::ImdbParser;
    auto thread = new QThread;

    parser->moveToThread(thread);

    connect(net, &Net::Imdb::Imdb::basicFetched, parser, &Library::ImdbParser::parse, Qt::QueuedConnection);
    connect(net, &Net::Imdb::Imdb::error, this, &Imdb::handleError);
    connect(net, &Net::Imdb::Imdb::error, thread, &QThread::quit);
    connect(net, &Net::Imdb::Imdb::progress, [this](quint64 recieved, quint64 total) {
        Q_EMIT progress(QString("Downloading %1:%2").arg(recieved).arg(total));
    });

    connect(parser, &Library::ImdbParser::parsed, this, &Imdb::handleParsed, Qt::QueuedConnection);
    connect(parser, &Library::ImdbParser::finished, this, &Imdb::handleFinished, Qt::QueuedConnection);
    connect(parser, &Library::ImdbParser::error, this, &Imdb::handleError, Qt::QueuedConnection);
    connect(parser, &Library::ImdbParser::error, thread, &QThread::quit, Qt::QueuedConnection);
    connect(parser, &Library::ImdbParser::finished, thread, &QThread::quit, Qt::QueuedConnection);
    connect(parser, &Library::ImdbParser::progress, [this](quint64 read, quint64 total) {
        Q_EMIT progress(QString("Parsing %1:%2").arg(read).arg(total));
    });

    connect(thread, &QThread::finished, parser, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();
    net->fetchBasics();
}

quint32 Rd::Application::Init::Imdb::createGenre(const QString& genre) {
    m_createGenre.bindValue(":name", QVariant(genre));
    if (!m_createGenre.exec()) {
        Q_EMIT error(m_createGenre.lastError().text());
        return 0;
    }
    QVariant idVariant = m_createGenre.lastInsertId();
    if (!idVariant.isValid()) {
        Q_EMIT error(QString("Unable to create %1").arg(genre));
        return 0;
    }
    return idVariant.toUInt();
}

QSqlError Rd::Application::Init::Imdb::createLink(const QString& imdb, quint32 id) {
    m_createLink.bindValue(":imdb", QVariant(imdb));
    m_createLink.bindValue(":genre", id);
    if (!m_createLink.exec()) return m_createLink.lastError();
    return QSqlError();
}

void Rd::Application::Init::Imdb::handleFinished() {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("INSERT INTO imdb_update(updated) VALUES(:updated)");
    query.bindValue(":updated", QVariant(QDateTime::currentDateTime().toString()));
    if (!query.exec()) {
        Q_EMIT error(query.lastError().text());
        db.rollback();
        return;
    }

    if (!db.commit()) {
        Q_EMIT error(db.lastError().text());
        return;
    }

    Q_EMIT done();
}

void Rd::Application::Init::Imdb::handleError(const QString& text) {
    QSqlDatabase db = QSqlDatabase::database();
    db.rollback();

    Q_EMIT error(text);
}

void Rd::Application::Init::Imdb::handleParsed(const QList<ImdbGenres>& lines) {
    for (auto& item : lines) {
        for (auto& genre : item.genres) {
            if (m_genres.contains(genre)) {
                    QSqlError err = createLink(item.id, m_genres.value(genre));
                    if (err.type() != QSqlError::NoError) {
                        Q_EMIT error(err.text());
                        return;
                    }
            } else {
                quint32 id = createGenre(genre);
                if (id != 0) {
                    m_genres.insert(genre, id);
                    QSqlError err = createLink(item.id, id);
                    if (err.type() != QSqlError::NoError) {
                        Q_EMIT error(err.text());
                        return;
                    }
                }
            }
        }
    }
}
