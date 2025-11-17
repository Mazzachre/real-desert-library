#pragma once

#include <QObject>
#include <QMap>
#include <QString>
#include <QSqlError>
#include "../model/genre.h"

namespace Rd {
    namespace Database {
        class Genres : public QObject {
            Q_OBJECT
        public:
            explicit Genres(QObject* parent = nullptr);
            ~Genres() noexcept;

            QSqlError getGenres(QList<Genre>& genres);
            QSqlError addGenre(const QString& genre, Genre& added);
        };
    }
}
