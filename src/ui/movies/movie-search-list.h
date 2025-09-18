#pragma once

#include <QAbstractListModel>
#include <QList>
#include "../../net/tmdb/movie.h"
#include "../../model/movie-search-result.h"

namespace Rd {
    namespace Ui {
        namespace Movies {
            class MovieSearchList : public QAbstractListModel {
                Q_OBJECT
                Q_PROPERTY(bool useOriginalTitle READ useOriginalTitle WRITE setUseOriginalTitle NOTIFY useOriginalTitleChanged)
                Q_PROPERTY(int page READ page NOTIFY resultsUpdated)
                Q_PROPERTY(int total READ total NOTIFY resultsUpdated)
            public:
                enum MoviesRoles {
                    IdRole = Qt::UserRole + 1,
                    TitleRole,
                    OriginalLanguageRole,
                    OriginalTitleRole,
                    OverviewRole,
                    PopularityRole,
                    PosterPathRole,
                    ReleaseYearRole
                };

                Q_SLOT void search(const QString& name, quint16 page = 1);
                Q_SLOT void clear();

                bool useOriginalTitle() const;
                void setUseOriginalTitle(bool useOriginalTitle);
                Q_SIGNAL void useOriginalTitleChanged();

                int page() const;
                int total() const;
                Q_SIGNAL void resultsUpdated();

                explicit MovieSearchList(QObject* parent = nullptr);
                ~MovieSearchList() noexcept;

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                Rd::Net::Tmdb::Movie* m_movie;
                QList<MovieSearchResult> m_results;
                quint32 m_page = 0;
                quint32 m_total = 0;
                bool m_useOriginalTitle;

                Q_SLOT void setResults(const QJsonObject& result);

            };
        }
    }
}
