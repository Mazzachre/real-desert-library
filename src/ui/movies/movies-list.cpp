#include "movies-list.h"

Rd::Ui::Movies::MoviesList::MoviesList(QObject* parent)
: QAbstractListModel(parent) {
}

Rd::Ui::Movies::MoviesList::~MoviesList() noexcept {
}

void Rd::Ui::Movies::MoviesList::setMovies(const QList<MovieListItem>& movies) {
    beginResetModel();
    m_movies = movies;
    endResetModel();
}

void Rd::Ui::Movies::MoviesList::clear() {
    beginResetModel();
    m_movies.clear();
    endResetModel();
}

QHash<int, QByteArray> Rd::Ui::Movies::MoviesList::roleNames() const {
    return {
        {IdRole, "id"},
        {TitleRole, "title"},
        {OriginalTitleRole, "originalTitle"},
        {ReleaseYearRole, "releaseYear"},
    };
}

int Rd::Ui::Movies::MoviesList::rowCount(const QModelIndex& parent) const {
    return m_movies.count();
}

QVariant Rd::Ui::Movies::MoviesList::data(const QModelIndex& index, int role) const {
    if (index.isValid()) {
        const MovieListItem& result = m_movies[index.row()];
        switch(role) {
            case IdRole:
                return result.id;
            case TitleRole:
                return result.title;
            case OriginalTitleRole:
                return result.originalTitle;
            case ReleaseYearRole:
                return result.releaseDate.year();
        }
    }
    return QVariant();
}
