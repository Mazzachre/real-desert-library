#pragma once

#include <QObject>
#include <QList>
#include "../db/shows.h"
#include "../db/episodes.h"
#include "../net/tmdb/show.h"
#include "../model/show.h"
#include "../model/episode.h"

namespace Rd {
    namespace Library {
        class AddShow : public QObject {
            Q_OBJECT
        public:
            explicit AddShow(QObject* parent = nullptr);
            ~AddShow() noexcept;

            Q_SLOT void addShow(quint32 id);
            Q_SIGNAL void showAdded(quint32 id);

            Q_SIGNAL void error(const QString& header, const QString& body);
        private:
            Rd::Database::Shows* m_show_db;
            Rd::Database::Episodes* m_episode_db;
            Rd::Net::Tmdb::Show* m_show_net;

            Show m_add;
            QList<quint8> m_seasons;
            QList<Episode> m_episodes;

            Q_SLOT void handleShowResult(const QJsonObject& result);
            Q_SLOT void handleEpisodeResult(const QJsonObject& result);
        };
    }
}
