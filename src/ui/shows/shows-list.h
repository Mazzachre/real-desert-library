#pragma once

#include <QAbstractListModel>
#include <QList>
#include "model/show-list-item.h"

namespace Rd {
    namespace Ui {
        namespace Shows {
            class ShowsList : public QAbstractListModel {
                Q_OBJECT
            public:
                enum ShowsRoles {
                    IdRole = Qt::UserRole + 1,
                    NameRole,
                    OriginalNameRole,
                    ReleaseYearRole,
                    EpisodeCountRole,
                    OfficialEpisodeCountRole
                };

                explicit ShowsList(QObject* parent = nullptr);
                ~ShowsList() noexcept;

                void setShows(const QList<ShowListItem>& shows);
                void clear();

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
            private:
                QList<ShowListItem> m_shows;
            };
        }
    }
}
