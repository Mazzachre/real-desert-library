#include "movies.h"

Rd::Ui::Movies::Movies::Movies(QObject* parent)
: QObject(parent)
, m_searchResults{new MovieSearchList} {
    connect(m_searchResults, &MovieSearchList::error, this, [this](const QString& header, const QString& body) {
        emit error(header, body);
    });
}

Rd::Ui::Movies::Movies::~Movies() noexcept {
    m_searchResults->deleteLater();
}

QAbstractListModel * Rd::Ui::Movies::Movies::searchResults() {
    return m_searchResults;
}

void Rd::Ui::Movies::Movies::init() {
}


void Rd::Ui::Movies::Movies::addMovie(quint32 id) {
    m_searchResults->clear();

    Q_EMIT movieSelected(id);
}

void Rd::Ui::Movies::Movies::load() {
    qDebug() << "Load movies";
}

void Rd::Ui::Movies::Movies::clear() {
    m_searchResults->clear();
}
