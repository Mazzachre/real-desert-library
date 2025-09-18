#pragma once

#include <QObject>
#include "movie-search-list.h"

namespace Rd {
    namespace Ui {
        namespace Movies {
            class Movies : public QObject {
                Q_OBJECT
                Q_PROPERTY(QAbstractListModel* searchResults READ searchResults NOTIFY searchResultsUpdated)
            public:
                explicit Movies(QObject* parent = nullptr);
                ~Movies() noexcept;

                void init();

                QAbstractListModel* searchResults();
                Q_INVOKABLE void addMovie(quint32 id);

                Q_SIGNAL void searchResultsUpdated();
                Q_SIGNAL void movieSelected(quint32 id);

                void clear();

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                MovieSearchList* m_searchResults;

                void load();
            };
        }
    }
}
