#pragma once

#include <QObject>
#include "../../db/shows.h"
#include "../../db/episodes.h"
#include "../../model/show.h"
#include "episode-list.h"
#include "episode-match.h"
#include "episode-details.h"

namespace Rd {
    namespace Ui {
        namespace Show {
            class Show : public QObject {
                Q_OBJECT
                Q_PROPERTY(QString name READ name NOTIFY showLoaded)
                Q_PROPERTY(QString originalName READ originalName NOTIFY showLoaded)
                Q_PROPERTY(QString overview READ overview NOTIFY showLoaded)
                Q_PROPERTY(QAbstractListModel* episodes READ episodes NOTIFY showLoaded)
                Q_PROPERTY(QAbstractListModel* matcher READ matcher NOTIFY showLoaded)
                Q_PROPERTY(QObject* details READ details NOTIFY showLoaded)
            public:
                explicit Show(QObject* parent = nullptr);
                ~Show() noexcept;

                Q_SLOT void loadShow(quint32 id);
                void clear();
                Q_SIGNAL void showLoaded();

                QString name();
                QString originalName();
                QString overview();
                QAbstractListModel* episodes();
                QAbstractListModel* matcher();
                QObject* details();

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                Rd::Database::Shows* m_shows_db;
                Rd::Database::Episodes* m_episodes_db;
                ::Show m_show;
                EpisodeList* m_episodes;
                EpisodeMatch* m_matcher;
                EpisodeDetails* m_details;

                Q_SLOT void handleEpisodesMatched(QMap<quint32, quint32> matches);
                Q_SLOT void handleFavoriteSet(quint32 id, bool favorite);
                void loadEpisodes();
           };
        }
    }
}
