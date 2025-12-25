#pragma once

#include <QObject>
#include <QJsonObject>
#include "db/movies.h"
#include "net/tmdb/movie.h"

namespace Rd {
    namespace Library {
        class AddMovie : public QObject {
            Q_OBJECT
        public:
            explicit AddMovie(QObject* parent = nullptr);
            ~AddMovie() noexcept;

            Q_SLOT void addMovie(quint32 id);
            Q_SIGNAL void movieAdded(quint32 id);

            Q_SIGNAL void error(const QString& header, const QString& body);
        private:
            Database::Movies* m_movie_db;
            Net::Tmdb::Movie* m_movie_net;

            Q_SLOT void handleMovieResult(const QJsonObject& result);
        };
    }
}
