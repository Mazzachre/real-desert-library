#pragma once

#include <QObject>
#include <QSqlError>
#include <QSqlDatabase>
#include <QList>
#include "cast-crew.h"
#include "playbacks.h"
#include "../model/episode.h"
#include "../model/episode-list-item.h"

namespace Rd {
    namespace Database {
        class Episodes : public QObject {
            Q_OBJECT
        public:
            explicit Episodes(QObject* parent = nullptr);
            ~Episodes() noexcept;

            QSqlError saveEpisodes(const QList<Episode>& episodes);
            QSqlError linkEpisodes(const QMap<quint32, quint32>& matches);
            QSqlError loadEpisodes(const quint32 id, QList<EpisodeListItem>& episodes);
            QSqlError setFavorite(quint32 id, bool favorite);
        private:
            CastCrew* m_castCrew;
            Playbacks* m_playbacks;

            using CreateFn = QSqlError (Episodes::*)(const QSqlDatabase&, const Episode&);
            std::vector<CreateFn> m_create_functions;

            QSqlError createEpisode(const QSqlDatabase& db, const Episode& episode);
            QSqlError createEpisodeCast(const QSqlDatabase& db, const Episode& episode);
            QSqlError createEpisodeCrew(const QSqlDatabase& db, const Episode& episode);

            QSqlError load(const QSqlDatabase& db, const quint32 id, QList<EpisodeListItem>& episodes);

            using LoadFn = QSqlError (Episodes::*)(const QSqlDatabase&, EpisodeListItem&);
            std::vector<LoadFn> m_load_functions;

            QSqlError loadFile(const QSqlDatabase& db, EpisodeListItem& episode);
            QSqlError loadCastAndCrew(const QSqlDatabase& db, EpisodeListItem& episode);
            QSqlError loadPlayback(const QSqlDatabase& db, EpisodeListItem& episode);

            QSqlError getMetaData(const QVariant& map, const QVariant& selected, QVariantMap& result);
        };
    }
}
