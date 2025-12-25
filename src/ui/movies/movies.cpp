#include "movies.h"
#include "model/movie-list-item.h"

Rd::Ui::Movies::Movies::Movies(QObject* parent)
: QObject(parent)
, m_addMovie{new Library::AddMovie}
, m_searchResults{new MovieSearchList}
, m_moviesList{new MoviesList}
, m_moviesFilter{new MoviesFilter}
, m_db{new Database::Movies} {
    connect(m_addMovie, &Library::AddMovie::movieAdded, this, [this](quint32 id) {
        Q_EMIT movieSelected(id);
    });
    connect(m_addMovie, &Library::AddMovie::error, this, [this](const QString& header, const QString& body) {
        Q_EMIT warn(header + ": " + body);
    });
    connect(m_searchResults, &MovieSearchList::error, this, [this](const QString& header, const QString& body) {
        Q_EMIT warn(header + ": " + body);
    });
    connect(m_moviesFilter, &MoviesFilter::error, this, [this](const QString& header, const QString& body) {
        Q_EMIT warn(header + ": " +  body);
    });
    connect(m_moviesFilter, &MoviesFilter::filterUpdated, this, [this]() {
        load();
    });
    m_order = Enums::SortOrder::Order::TitleAsc;
}

Rd::Ui::Movies::Movies::~Movies() noexcept {
    m_addMovie->deleteLater();
    m_searchResults->deleteLater();
    m_moviesFilter->deleteLater();
    m_moviesList->deleteLater();
    m_db->deleteLater();
}

void Rd::Ui::Movies::Movies::init() {
    m_moviesFilter->init();
    Q_EMIT filterUpdated();
}

void Rd::Ui::Movies::Movies::clear() {
    m_searchResults->clear();
    m_moviesList->clear();
    m_moviesFilter->clear();
}

void Rd::Ui::Movies::Movies::addMovie(quint32 id) {
    m_searchResults->clear();
    m_addMovie->addMovie(id);
}

QAbstractListModel * Rd::Ui::Movies::Movies::searchResults() {
    return m_searchResults;
}

QAbstractListModel * Rd::Ui::Movies::Movies::list() {
    return m_moviesList;
}

Rd::Ui::Movies::MoviesFilter* Rd::Ui::Movies::Movies::filter() {
    return m_moviesFilter;
}

Rd::Enums::SortOrder::Order Rd::Ui::Movies::Movies::order() {
    return m_order;
}

void Rd::Ui::Movies::Movies::setOrder(Enums::SortOrder::Order order) {
    m_order = order;
    load();
    Q_EMIT orderUpdated();
}


void Rd::Ui::Movies::Movies::load() {
    QList<MovieListItem> movies;
    QSqlError err = m_db->findMovies(m_moviesFilter->filter(), m_order, movies);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading movie list", err.text());
        return;
    }

    m_moviesList->setMovies(movies);
    Q_EMIT listUpdated();
}
