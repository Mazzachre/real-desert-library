#include "episode-list.h"
#include "../file-handler.h"
#include "../mode-handler.h"

Rd::Ui::Show::EpisodeList::EpisodeList(QObject* parent)
: QAbstractListModel(parent) {
    m_playables = true;
}

Rd::Ui::Show::EpisodeList::~EpisodeList() noexcept {
}

void Rd::Ui::Show::EpisodeList::setEpisodes(const QList<EpisodeListItem>& episodes) {
    m_episodes = episodes;
    filterEpisodes();
}

void Rd::Ui::Show::EpisodeList::clear() {
    beginResetModel();
    m_episodes.clear();
    m_filtered.clear();
    m_playables = true;
    m_favorites = false;
    m_selected = 0;
    endResetModel();
    Q_EMIT favoritesUpdated();
    Q_EMIT playablesUpdated();
}

void Rd::Ui::Show::EpisodeList::toggleFavorite(quint32 id) {
    for (auto& item: m_episodes) {
        if (item.id == id) {
            Q_EMIT favoriteSet(id, !item.favorite);
            break;
        }
    }
}

void Rd::Ui::Show::EpisodeList::updateFavorite(quint32 id, bool favorite) {
    for (auto& item: m_episodes) {
        if (item.id == id) {
            if (item.favorite == favorite) break;
            item.favorite = favorite;

            if (m_favorites) {
                if (favorite) {
                    addToFiltered(item);
                } else {
                    removeFromFiltered(id);
                }
            } else {
                updateFiltered(item);
            }

            break;
        }
    }
}

void Rd::Ui::Show::EpisodeList::openDetails(quint32 id) {
    for (auto& item: m_episodes) {
        if (item.id == id) {
            Q_EMIT detailsOpened(item);
            break;
        }
    }
}

bool Rd::Ui::Show::EpisodeList::favorites() {
    return m_favorites;
}

void Rd::Ui::Show::EpisodeList::setFavorites(bool favorites) {
    m_favorites = favorites;
    filterEpisodes();
    Q_EMIT favoritesUpdated();
}

bool Rd::Ui::Show::EpisodeList::playables() {
    return m_playables;
}

void Rd::Ui::Show::EpisodeList::setPlayables(bool playables) {
    m_playables = playables;
    filterEpisodes();
    Q_EMIT playablesUpdated();
}

quint32 Rd::Ui::Show::EpisodeList::selected() {
    return m_selected;
}

void Rd::Ui::Show::EpisodeList::setSelected(quint32 selected) {
    for (const auto& episode : m_episodes) {
        if (episode.id == selected) {
            if (isPlayable(episode.path)) {
                m_selected = selected;
                Q_EMIT selectedUpdated();
            }
            break;
        }
    }
}

QHash<int, QByteArray> Rd::Ui::Show::EpisodeList::roleNames() const {
    return {
        {IdRole, "id"},
        {PlayableRole, "playable"},
        {SeasonRole, "season"},
        {EpisodeRole, "episode"},
        {NameRole, "name"},
        {FavoriteRole, "favorite"},
        {RuntimeRole, "runtime"},
        {SubtitlesRole, "subtitles"},
        {PlayedRole, "played"},
    };
}

int Rd::Ui::Show::EpisodeList::rowCount(const QModelIndex& parent) const {
    return m_filtered.count();
}

QVariant Rd::Ui::Show::EpisodeList::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        const EpisodeListItem& item = m_filtered[index.row()];
        switch(role) {
            case IdRole:
                return item.id;
            case PlayableRole:
                return isPlayable(item.path);
            case SeasonRole:
                return static_cast<int>(item.season);
            case EpisodeRole:
                return static_cast<int>(item.episode);
            case NameRole:
                return item.name;
            case FavoriteRole:
                return item.favorite;
            case RuntimeRole: {
                QString runtime = QString("%1:%2")
                    .arg(item.actualRuntime / 60)
                    .arg(item.actualRuntime % 60, 2, 10, QChar('0'));
                return runtime;
            }
            case SubtitlesRole:
                return !item.subtitles.isEmpty();
            case PlayedRole:
                return QVariant();
        }
    }

    return QVariant();
}

void Rd::Ui::Show::EpisodeList::filterEpisodes() {
    beginResetModel();
    m_filtered.clear();
    for (const auto& episode : m_episodes) {
        if (m_favorites) {
            if (episode.favorite) m_filtered << episode;
        } else if (m_playables) {
            if (isPlayable(episode.path)) m_filtered << episode;
        } else {
            m_filtered << episode;
        }
    }
    endResetModel();
}

void Rd::Ui::Show::EpisodeList::addToFiltered(const EpisodeListItem& episode) {
    int insertIndex = m_filtered.count(); // Default to inserting at the end

    for (int i = 0; i < m_filtered.count(); ++i) {
        const auto& item = m_filtered[i];

        if (item.season > episode.season ||
           (item.season == episode.season && item.episode > episode.episode)) {
            insertIndex = i;
            break;
        }
    }

    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    m_filtered.insert(insertIndex, episode);
    endInsertRows();
}

void Rd::Ui::Show::EpisodeList::removeFromFiltered(quint32 id) {
    for (int i = 0; i < m_filtered.count(); ++i) {
        auto& item = m_filtered[i];
        if (item.id == id) {
            beginRemoveRows(QModelIndex(), i, i);
            m_filtered.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void Rd::Ui::Show::EpisodeList::updateFiltered(const EpisodeListItem& episode) {
    for (int i = 0; i < m_filtered.count(); ++i) {
        auto& item = m_filtered[i];
        if (item.id == episode.id) {
            item = episode;
            dataChanged(index(i), index(i), {FavoriteRole});
            break;
        }
    }
}

bool Rd::Ui::Show::EpisodeList::isPlayable(const QString& path) const {
    if (path.isEmpty()) return false;
    return true;
}
