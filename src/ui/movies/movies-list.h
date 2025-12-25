#pragma once

#include <QAbstractListModel>
#include "model/movie-list-item.h"

namespace Rd {
    namespace Ui {
        namespace Movies {
            class MoviesList : public QAbstractListModel {
                Q_OBJECT
            public:
                enum MoviesRoles {
                    IdRole = Qt::UserRole + 1,
                    TitleRole,
                    OriginalTitleRole,
                    ReleaseYearRole,
                };

                explicit MoviesList(QObject* parent = nullptr);
                ~MoviesList() noexcept;

                void setMovies(const QList<MovieListItem>& movies);
                void clear();

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
            private:
                QList<MovieListItem> m_movies;
            };
        }
    }
}
