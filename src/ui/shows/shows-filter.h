#pragma once

#include <QObject>
#include "shows-genre-list.h"
#include "shows-tags-list.h"
#include "../../model/show-filter.h"
#include "../../db/shows.h"

namespace Rd {
    namespace Ui {
        namespace Shows {
            class ShowsFilter : public QObject {
                Q_OBJECT
                Q_PROPERTY(quint16 newerThan READ newerThan WRITE setNewerThan NOTIFY filterUpdated)
                Q_PROPERTY(quint16 olderThan READ olderThan WRITE setOlderThan NOTIFY filterUpdated)
                Q_PROPERTY(QAbstractListModel* genres READ genres NOTIFY filterUpdated)
                Q_PROPERTY(QAbstractListModel* tags READ tags NOTIFY filterUpdated)
            public:
                explicit ShowsFilter(QObject* parent = nullptr);
                ~ShowsFilter() noexcept;

                void init();
                void clear();

                quint16 newerThan();
                void setNewerThan(quint16 value);

                quint16 olderThan();
                void setOlderThan(quint16 value);

                QAbstractListModel* genres();
                QAbstractListModel* tags();

                ShowFilter filter();
                Q_SIGNAL void filterUpdated();

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                Rd::Database::Shows* m_db;
                ShowGenreList* m_genres;
                ShowTagsList* m_tags;
                ShowFilter m_filter;
            };
        }
    }
}
