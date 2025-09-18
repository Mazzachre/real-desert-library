#pragma once

#include <QAbstractListModel>
#include <QList>
#include "../../net/tmdb/show.h"
#include "../../model/show-search-result.h"

namespace Rd {
    namespace Ui {
        namespace Shows {
            class ShowSearchList : public QAbstractListModel {
                Q_OBJECT
                Q_PROPERTY(bool useOriginalName READ useOriginalName WRITE setUseOriginalName NOTIFY useOriginalNameChanged)
                Q_PROPERTY(int page READ page NOTIFY resultsUpdated)
                Q_PROPERTY(int total READ total NOTIFY resultsUpdated)
            public:
                enum ShowsRoles {
                    IdRole = Qt::UserRole + 1,
                    NameRole,
                    OriginalLanguageRole,
                    OriginalNameRole,
                    OverviewRole,
                    PopularityRole,
                    PosterPathRole,
                    ReleaseYearRole
                };

                Q_SLOT void search(const QString& name, quint16 page = 1);
                void clear();

                bool useOriginalName() const;
                void setUseOriginalName(bool useOriginalName);
                Q_SIGNAL void useOriginalNameChanged();

                int page() const;
                int total() const;
                Q_SIGNAL void resultsUpdated();

                explicit ShowSearchList(QObject* parent = nullptr);
                ~ShowSearchList() noexcept;

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                Rd::Net::Tmdb::Show* m_show;
                QList<ShowSearchResult> m_results;
                quint32 m_page = 0;
                quint32 m_total = 0;
                bool m_useOriginalName;

                Q_SLOT void setResults(const QJsonObject& result);
            };
        }
    }
}
