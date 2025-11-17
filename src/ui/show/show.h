#pragma once

#include <QObject>
#include "db/shows.h"
#include "db/episodes.h"
#include "db/files.h"
#include "db/genres.h"
#include "db/tags.h"
#include "db/playbacks.h"
#include "model/show.h"
#include "lib/local-path.h"
#include "model/genre.h"
#include "episode-list.h"
#include "episode-match.h"
#include "episode-details.h"
#include "show-details.h"
#include "cast-list.h"

namespace Rd {
    namespace Ui {
        namespace Show {
            class Show : public QObject {
                Q_OBJECT
                Q_PROPERTY(QAbstractListModel* episodes READ episodes NOTIFY showLoaded)
                Q_PROPERTY(QAbstractListModel* matcher READ matcher NOTIFY showLoaded)
                Q_PROPERTY(QAbstractListModel* cast READ cast NOTIFY showLoaded)
                Q_PROPERTY(QObject* episode READ episode NOTIFY showLoaded)
                Q_PROPERTY(QObject* show READ show NOTIFY showLoaded)
                Q_PROPERTY(QVariantList genres READ genres NOTIFY genresUpdated)
                Q_PROPERTY(QVariantList tags READ tags NOTIFY tagsUpdated)
            public:
                explicit Show(QObject* parent = nullptr);
                ~Show() noexcept;

                Q_SLOT void load(quint32 id);
                void loadByEpisodeId(quint32 id);
                void clear();
                Q_SIGNAL void showLoaded();
                Q_SIGNAL void genresUpdated();
                Q_SIGNAL void tagsUpdated();

                QAbstractListModel* episodes();
                QAbstractListModel* matcher();
                QAbstractListModel* cast();
                QObject* episode();
                QObject* show();
                QVariantList genres();
                QVariantList tags();

                Q_INVOKABLE void setFavorite(quint32 id, bool favorite);
                Q_INVOKABLE void setSubtitle(quint32 fileId, const QUrl& subtitle);
                Q_INVOKABLE void setPlayed(quint32 fileId, quint32 played);
                Q_INVOKABLE void removeGenre(quint32 id);
                Q_INVOKABLE void addGenre(const QVariantMap& genre);
                Q_INVOKABLE void removeTag(quint32 id);
                Q_INVOKABLE void addTag(const QVariantMap& tag);

                Q_SIGNAL void error(const QString& header, const QString& body);
                Q_SIGNAL void warn(const QString& text);
            private:
                Library::LocalPath* m_localPath;
                Database::Shows* m_shows_db;
                Database::Episodes* m_episodes_db;
                Database::Files* m_files_db;
                Database::Genres* m_genre_db;
                Database::Tags* m_tags_db;
                Database::Playbacks* m_play_db;

                ShowDetails* m_show_details;
                EpisodeList* m_episodes;
                EpisodeMatch* m_matcher;
                EpisodeDetails* m_details;
                CastList* m_cast;

                ::Show m_show;
                QList<Genre> m_genres;
                QVariantList m_tags;

                Q_SLOT void handleEpisodesMatched(QMap<quint32, quint32> matches);
                Q_SIGNAL void episodesLoaded(const QList<EpisodeListItem>& episodes);
                Q_SIGNAL void favoriteSet(quint32 id, bool favorite);
                Q_SIGNAL void subtitleSet(quint32 id, const QString& subtitle);
                Q_SIGNAL void playedSet(const Playback& playback);

                void loadEpisodes();
                void loadGenres();
                void loadTags();
                void linkGenre(quint32 id);
                void linkTag(quint32 id);
           };
        }
    }
}
