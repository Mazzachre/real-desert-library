#pragma once

#include <QObject>
#include <QList>
#include <QSqlError>
#include <QSqlDatabase>
#include "model/movie-list-item.h"
#include "model/movie-filter.h"
#include "enums/sort-order.h"

namespace Rd {
    namespace Database {
        class Movies : public QObject {
            Q_OBJECT
        public:
            explicit Movies(QObject* parent = nullptr);
            ~Movies() noexcept;

            QSqlError exists(const quint32 id, bool& exists);
            QSqlError findMovies(const MovieFilter& filter, Rd::Enums::SortOrder::Order order, QList<MovieListItem>& result);
        };
    }
}
