#include "add-movie.h"
#include "ui/busy-handler.h"

Rd::Library::AddMovie::AddMovie(QObject* parent)
: QObject(parent)
, m_movie_db{new Database::Movies}
, m_movie_net{new Net::Tmdb::Movie} {
    connect(m_movie_net, &Net::Tmdb::Movie::getResult, this, &AddMovie::handleMovieResult);
    connect(m_movie_net, &Net::Tmdb::Movie::error, this, [this](const QString& text) {
        Q_EMIT error(u"Error adding movie: "_qs, text);
    });
}

Rd::Library::AddMovie::~AddMovie() noexcept {
    m_movie_db->deleteLater();
}

void Rd::Library::AddMovie::addMovie(quint32 id) {
    bool exists = false;
    QSqlError err = m_movie_db->exists(id, exists);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error adding show", err.text());
        return;
    }
    if (exists) {
        Q_EMIT movieAdded(id);
        return;
    }
    m_movie_net->get(id);
    Ui::BusyHandler::instance()->setBusy(true);
}

void Rd::Library::AddMovie::handleMovieResult(const QJsonObject& result) {
    qDebug() << result;
}
