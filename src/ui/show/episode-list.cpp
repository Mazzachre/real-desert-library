#include "episode-list.h"
#include "ui/file-handler.h"
#include "ui/mode-handler.h"
#include "lib/format.h"
#include <bits/stdc++.h>
#include <QSet>

Rd::Ui::Show::EpisodeList::EpisodeList(QObject* parent)
: QAbstractListModel(parent)
, m_localPath{new Rd::Library::LocalPath} {
    m_playables = true;
}

Rd::Ui::Show::EpisodeList::~EpisodeList() noexcept {
}

void Rd::Ui::Show::EpisodeList::setEpisodes(const QList<EpisodeListItem>& episodes) {
    m_episodes = episodes;
    filterEpisodes();
    QSet<quint16> seasons;
    for (auto& episode : m_episodes) {
        if (!seasons.contains(episode.season)) {
            seasons << episode.season;
            QVariantMap map = QVariantMap({
                {"id", episode.season},
                {"name", QString("Season %1").arg(episode.season)},
                {"selected", false}
            });
            m_seasons << map;
        }
    }
    Q_EMIT seasonsUpdated();
}

void Rd::Ui::Show::EpisodeList::clear() {
    beginResetModel();
    m_episodes.clear();
    m_filtered.clear();
    m_seasons.clear();
    m_playables = true;
    m_favorites = false;
    m_selected = 0;
    endResetModel();
    Q_EMIT favoritesUpdated();
    Q_EMIT playablesUpdated();
    Q_EMIT seasonsUpdated();
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

void Rd::Ui::Show::EpisodeList::updateSubtitle(quint32 fileId, const QString& subtitle) {
    for (auto& item : m_episodes) {
        if (item.fileId == fileId) {
            item.selectedSubtitle = subtitle;
            QUrl url(subtitle);
            QFileInfo info(url.fileName());
            item.subtitles = QVariantMap({
                {"type", "external"},
                {"file", info.completeBaseName()},
                {"ext", info.suffix()}
            });
            break;
        }
    }
    for (int i = 0; i < m_filtered.count(); ++i) {
        auto& item = m_filtered[i];
        if (item.fileId == fileId) {
            item.selectedSubtitle = subtitle;
            Q_EMIT dataChanged(index(i), index(i), {SubtitleSelectedRole});
            break;
        }
    }
}

void Rd::Ui::Show::EpisodeList::updatePlayed(const Playback& playback) {
    for (auto& item : m_episodes) {
        if (item.fileId == playback.fileId) {
            item.playback.append(playback);
            break;
        }
    }
    for (int i = 0; i < m_filtered.count(); ++i) {
        auto& item = m_filtered[i];
        if (item.fileId == playback.fileId) {
            item.playback.append(playback);
            Q_EMIT dataChanged(index(i), index(i), {PlayedRole, PlaybackListRole, PlayedFullyRole});
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
            if (episode.isPlayable()) {
                m_selected = selected;
                Q_EMIT selectedUpdated();
            }
            break;
        }
    }
}

QVariantList Rd::Ui::Show::EpisodeList::seasons() {
    return m_seasons;
}

void Rd::Ui::Show::EpisodeList::toggleSeason(quint16 season) {
    for (auto& item : m_seasons) {
        auto map = item.toMap();
        if (map["id"].toUInt() == season) {
            map["selected"] = !map["selected"].toBool();
            item = map;
            filterEpisodes();
            Q_EMIT seasonsUpdated();
            return;
        }
    }
}

void Rd::Ui::Show::EpisodeList::play(bool list) {
    if (list && m_selected > 0) {
        QList<EpisodeListItem> reordered;
        int index = -1;
        for (int i = 0; i < m_filtered.size(); ++i) {
            if (m_filtered[i].id == m_selected) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            for (int i = index; i < m_filtered.count(); ++i) {
                reordered.append(m_filtered[i]);
            }
            for (int i = 0; i < index; ++i) {
                reordered.append(m_filtered[i]);
            }
        }
        qDebug() << "Play" << reordered;
    } else if (list) {
        qDebug() << "Play" << m_filtered;
    } else if (m_selected > 0) {
        for (auto& item : m_filtered) {
            if (item.id == m_selected) {
                qDebug() << "Play" << item;
                break;
            }
        }
    } else {
        qDebug() << "Play" << m_filtered[0];
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
        {PathRole, "path"},
        {FileRole, "file"},
        {FileNameRole, "fileName"},
        {FileTimeRole, "fileRuntime"},
        {SubtitlesRole, "subtitles"},
        {SubtitleSelectedRole, "subtitleSelected"},
        {PlayedRole, "played"},
        {PlayedFullyRole, "playedFully"},
        {PlaybackListRole, "playbackList"},
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
                return item.isPlayable();
            case SeasonRole:
                return static_cast<int>(item.season);
            case EpisodeRole:
                return static_cast<int>(item.episode);
            case NameRole:
                return item.name;
            case FavoriteRole:
                return item.favorite;
            case RuntimeRole:
                return Library::formatRuntime(item.actualRuntime);
            case PathRole: {
                QUrl url(item.path);
                return QUrl::fromLocalFile(m_localPath->getLocalPath(url.adjusted(QUrl::RemoveFilename)));
            }
            case FileRole:
                return item.fileId;
            case FileNameRole: {
                QUrl url(item.path);
                return url.fileName();
            }
            case FileTimeRole:
                return static_cast<int>(item.actualRuntime);
            case SubtitlesRole:
                return !item.subtitles.isEmpty();
            case SubtitleSelectedRole:
                return item.selectedSubtitle;
            case PlayedRole:
                return !item.playback.isEmpty();
            case PlayedFullyRole: {
                if (item.playback.isEmpty()) return false;
                quint32 lastPlayed = item.playback[0].played;
                quint32 runtime = item.actualRuntime*60;
                quint32 diff = runtime - lastPlayed;
                return diff < runtime/10;
            }
            case PlaybackListRole: {
                QVariantList result;
                for (auto& playback : item.playback.first(std::min<int>(5, item.playback.count()))) {
                    result << playback.toMap(item.actualRuntime);
                }
                return result;
            }
        }
    }

    return QVariant();
}

void Rd::Ui::Show::EpisodeList::filterEpisodes() {
    beginResetModel();
    m_filtered.clear();
    QSet<quint16> seasons;
    for (const auto& season : m_seasons) {
        auto map = season.toMap();
        if (map.value("selected").toBool()) {
            seasons << map.value("id").toUInt();
        }
    }
    for (const auto& episode : m_episodes) {
        if (m_favorites && !episode.favorite) continue;
        if (m_playables && !episode.isPlayable()) continue;
        if (!seasons.isEmpty() && !seasons.contains(episode.season)) continue;

        m_filtered << episode;
    }
    endResetModel();
}

void Rd::Ui::Show::EpisodeList::addToFiltered(const EpisodeListItem& episode) {
    int insertIndex = m_filtered.count();

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
