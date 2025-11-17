#pragma once

#include <QObject>
#include <QList>
#include <QMap>
#include <QSqlQuery>
#include <QSqlError>
#include "model/imdb-genres.h"

namespace Rd {
    namespace Application {
        namespace Init {
            class Imdb : public QObject {
                Q_OBJECT
            public:
                explicit Imdb(QObject* parent = nullptr);
                ~Imdb() noexcept;

                Q_SLOT void start();
                Q_SIGNAL void done();
                Q_SIGNAL void progress(const QString& text);
                Q_SIGNAL void error(const QString& text);
            private:
                QSqlQuery m_createGenre;
                QSqlQuery m_createLink;

                QMap<QString, quint32> m_genres;

                quint32 createGenre(const QString& genre);
                QSqlError createLink(const QString& imdb, quint32 id);

                Q_SLOT void handleFinished();
                Q_SLOT void handleError(const QString& text);
                Q_SLOT void handleParsed(const QList<ImdbGenres>& lines);
            };
        }
    }
}
