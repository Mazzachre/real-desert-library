#pragma once

#include <QObject>
#include <QList>
#include "../../model/episode-list-item.h"

namespace Rd {
    namespace Ui {
        namespace Show {
            class EpisodeDetails : public QObject {
                Q_OBJECT
                Q_PROPERTY(QString name READ name NOTIFY dataUpdated)
                Q_PROPERTY(bool favorite READ favorite NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList episode READ episode NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList file READ file NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList video READ video NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList audio READ audio NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList subtitle READ subtitle NOTIFY dataUpdated)
            public:
                explicit EpisodeDetails(QObject* parent = nullptr);
                ~EpisodeDetails() noexcept;

                Q_SIGNAL void dataUpdated();

                Q_SLOT void setEpisode(const EpisodeListItem& episode);
                Q_SIGNAL void open();

                Q_SLOT void setFavorite();
                Q_SIGNAL void favoriteSet(quint32 id, bool favorite);

                QString name();
                bool favorite();
                QVariantList episode();
                QVariantList file();
                QVariantList video();
                QVariantList audio();
                QVariantList subtitle();
            private:
                EpisodeListItem m_episode;
            };
        }
    }
}
