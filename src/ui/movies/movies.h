#pragma once

#include <QObject>
#include "movie-search-list.h"
#include "movies-list.h"
#include "movies-filter.h"
#include "enums/sort-order.h"
#include "lib/add-movie.h"
#include "db/movies.h"

namespace Rd {
    namespace Ui {
        namespace Movies {
            class Movies : public QObject {
                Q_OBJECT
                Q_PROPERTY(QAbstractListModel* searchResults READ searchResults NOTIFY searchResultsUpdated)
                Q_PROPERTY(QAbstractListModel* list READ list NOTIFY listUpdated)
                Q_PROPERTY(MoviesFilter* filter READ filter NOTIFY filterUpdated)
                Q_PROPERTY(Enums::SortOrder::Order order READ order WRITE setOrder NOTIFY orderUpdated)
            public:
                explicit Movies(QObject* parent = nullptr);
                ~Movies() noexcept;

                void init();

                Q_INVOKABLE void addMovie(quint32 id);
                void clear();

                QAbstractListModel* searchResults();
                Q_SIGNAL void searchResultsUpdated();

                QAbstractListModel* list();
                Q_SIGNAL void listUpdated();

                Q_SIGNAL void movieSelected(quint32 id);

                MoviesFilter* filter();
                Q_SIGNAL void filterUpdated();

                Enums::SortOrder::Order order();
                void setOrder(Enums::SortOrder::Order order);
                Q_SIGNAL void orderUpdated();

                Q_SIGNAL void error(const QString& header, const QString& body);
                Q_SIGNAL void warn(const QString& text);
            private:
                Library::AddMovie* m_addMovie;
                MovieSearchList* m_searchResults;
                MoviesList* m_moviesList;
                MoviesFilter* m_moviesFilter;
                Enums::SortOrder::Order m_order;
                Database::Movies* m_db;

                void load();
            };
        }
    }
}
