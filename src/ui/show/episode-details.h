#pragma once

#include <QObject>
#include <QList>
#include "model/episode-list-item.h"

namespace Rd {
    namespace Ui {
        namespace Show {
            class EpisodeDetails : public QObject {
                Q_OBJECT
                Q_PROPERTY(quint32 id READ id NOTIFY dataUpdated)
                Q_PROPERTY(QString name READ name NOTIFY dataUpdated)
                Q_PROPERTY(bool favorite READ favorite NOTIFY dataUpdated)
                Q_PROPERTY(bool playable READ playable NOTIFY dataUpdated)
                Q_PROPERTY(bool hasExtras READ hasExtras NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList episode READ episode NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList file READ file NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList video READ video NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList audio READ audio NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList subtitle READ subtitle NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList crew READ crew NOTIFY dataUpdated)
                Q_PROPERTY(QVariantList cast READ cast NOTIFY dataUpdated)
            public:
                explicit EpisodeDetails(QObject* parent = nullptr);
                ~EpisodeDetails() noexcept;

                Q_SIGNAL void dataUpdated();

                Q_SLOT void setEpisode(const EpisodeListItem& episode);
                Q_SIGNAL void open();
                void clear();

                Q_SLOT void updateFavorite(quint32 id, bool favorite);

                quint32 id();
                QString name();
                bool favorite();
                bool playable();
                bool hasExtras();
                QVariantList episode();
                QVariantList file();
                QVariantList video();
                QVariantList audio();
                QVariantList subtitle();
                QVariantList crew();
                QVariantList cast();
            private:
                EpisodeListItem m_episode;
            };
        }
    }
}
