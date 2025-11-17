#pragma once

#include <QAbstractListModel>
#include <QList>
#include "model/episode-list-item.h"

namespace Rd {
    namespace Ui {
        namespace Show {
            class CastList : public QAbstractListModel {
                Q_OBJECT
            public:
                enum RoleList {
                    IdRole = Qt::UserRole + 1,
                    NameRole,
                    CountRole,
                };

                explicit CastList(QObject* parent = nullptr);
                ~CastList();

                Q_SLOT void setEpisodes(const QList<EpisodeListItem>& episodes);
                void clear();

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
            private:
                QList<Person> m_people;
                QList<Person> m_filtered;
                QMap<quint32, quint32> m_count;
                QString m_filter;

                void filter();
            };
        }
    }
}
