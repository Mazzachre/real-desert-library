#include "shows-list.h"

Rd::Ui::Shows::ShowsList::ShowsList(QObject* parent)
: QAbstractListModel(parent) {
}

Rd::Ui::Shows::ShowsList::~ShowsList() noexcept {
}

void Rd::Ui::Shows::ShowsList::setShows(const QList<ShowListItem>& shows) {
    beginResetModel();
    m_shows = shows;
    endResetModel();
}

void Rd::Ui::Shows::ShowsList::clear() {
    beginResetModel();
    m_shows.clear();
    endResetModel();
}


QHash<int, QByteArray> Rd::Ui::Shows::ShowsList::roleNames() const {
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {OriginalNameRole, "originalName"},
        {ReleaseYearRole, "releaseYear"},
        {EpisodeCountRole, "episodeCount"},
        {OfficialEpisodeCountRole, "officialEpisodeCount"}
    };
}

int Rd::Ui::Shows::ShowsList::rowCount(const QModelIndex& parent) const {
    return m_shows.count();
}

QVariant Rd::Ui::Shows::ShowsList::data(const QModelIndex& index, int role) const {
    if (index.isValid()) {
        const ShowListItem& result = m_shows[index.row()];
        switch(role) {
            case IdRole:
                return result.id;
            case NameRole:
                return result.name;
            case OriginalNameRole:
                return result.originalName;
            case ReleaseYearRole:
                return result.firstAirDate.year();
            case EpisodeCountRole:
                return result.episodeCount;
            case OfficialEpisodeCountRole:
                return result.officialEpisodeCount;
        }
    }
    return QVariant();
}
