#pragma once

#include <QAbstractListModel>
#include <QList>
#include "model/episode-list-item.h"
#include "lib/local-path.h"

namespace Rd {
    namespace Ui {
        namespace Show {
            class EpisodeList : public QAbstractListModel {
                Q_OBJECT
                Q_PROPERTY(bool playables READ playables WRITE setPlayables NOTIFY playablesUpdated)
                Q_PROPERTY(bool favorites READ favorites WRITE setFavorites NOTIFY favoritesUpdated)
                Q_PROPERTY(quint32 selected READ selected WRITE setSelected NOTIFY selectedUpdated)
            public:
                enum RoleList {
                    IdRole = Qt::UserRole + 1,
                    PlayableRole,
                    SeasonRole,
                    EpisodeRole,
                    NameRole,
                    FavoriteRole,
                    RuntimeRole,
                    PathRole,
                    FileRole,
                    FileNameRole,
                    FileTimeRole,
                    SubtitlesRole,
                    SubtitleSelectedRole,
                    PlayedRole,
                    PlayedFullyRole,
                    PlaybackListRole,
                };

                explicit EpisodeList(QObject* parent = nullptr);
                ~EpisodeList() noexcept;

                Q_SLOT void setEpisodes(const QList<EpisodeListItem>& episodes);
                void clear();

                Q_SLOT void updateFavorite(quint32 id, bool favorite);
                Q_SLOT void updateSubtitle(quint32 fileId, const QString& subtitle);
                Q_SLOT void updatePlayed(const Playback& playback);

                Q_SLOT void openDetails(quint32 id);
                Q_SIGNAL void detailsOpened(const EpisodeListItem& episode);

                bool favorites();
                void setFavorites(bool favorites);
                Q_SIGNAL void favoritesUpdated();

                bool playables();
                void setPlayables(bool playables);
                Q_SIGNAL void playablesUpdated();

                quint32 selected();
                void setSelected(quint32 selected);
                Q_SIGNAL void selectedUpdated();

                Q_INVOKABLE void play(bool list = false);

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
            private:
                Rd::Library::LocalPath* m_localPath;

                bool m_favorites;
                bool m_playables;
                quint32 m_selected;
                QList<EpisodeListItem> m_episodes;
                QList<EpisodeListItem> m_filtered;

                void filterEpisodes();
                void addToFiltered(const EpisodeListItem& episode);
                void removeFromFiltered(quint32 id);
                void updateFiltered(const EpisodeListItem& episode);
            };
        }
    }
}
