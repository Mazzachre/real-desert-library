#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QString>
#include "../../model/file.h"
#include "../../model/episode-list-item.h"

namespace Rd {
    namespace Ui {
        namespace Show {
            class EpisodeMatch : public QAbstractListModel {
                Q_OBJECT
                Q_PROPERTY(QVariantList episodes READ episodes NOTIFY updated)
            public:
                enum RoleList {
                     IdRole = Qt::UserRole + 1,
                     PathRole,
                     FileRole,
                     //TODO META
                     RuntimeRole,
                     MatchRole
                };

                explicit EpisodeMatch(QObject* parent = nullptr);
                ~EpisodeMatch() noexcept;

                void setEpisodes(const QList<EpisodeListItem>& episodes);
                Q_INVOKABLE void clear();

                QVariantList episodes();
                Q_SIGNAL void updated();

                Q_SLOT void selected(quint32 fileId, quint32 episodeId);
                Q_SLOT void save();

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

                Q_SIGNAL void requestOpenDialog();
                Q_SIGNAL void episodesMatched(QMap<quint32, quint32> matches);
            private:
                QList<File> m_files;
                QVariantList m_episodes;
                QMap<quint32, quint32> m_matches;

                Q_SLOT void handleFilesAdded(const QList<QVariantMap>& files);
                QVariantMap findMatch(const QString& path);
                quint32 getEpisode(const QVariantMap& match);
            };
        }
    }
}
