#pragma once

#include <QObject>
#include "show-search-list.h"
#include "shows-list.h"
#include "shows-filter.h"
#include "lib/add-show.h"
#include "db/shows.h"
#include "enums/sort-order.h"

namespace Rd {
    namespace Ui {
        namespace Shows {
            class Shows : public QObject {
                Q_OBJECT
                Q_PROPERTY(QAbstractListModel* searchResults READ searchResults NOTIFY searchResultsUpdated)
                Q_PROPERTY(QAbstractListModel* list READ list NOTIFY listUpdated)
                Q_PROPERTY(ShowsFilter* filter READ filter NOTIFY filterUpdated)
                Q_PROPERTY(Enums::SortOrder::Order order READ order WRITE setOrder NOTIFY orderUpdated)
            public:
                explicit Shows(QObject* parent = nullptr);
                ~Shows() noexcept;

                void init();
                void clear();

                QAbstractListModel* searchResults();
                Q_SIGNAL void searchResultsUpdated();

                Q_INVOKABLE void addShow(quint32 tmdbId);
                Q_SIGNAL void showSelected(quint32 id);

                QAbstractListModel* list();
                Q_SIGNAL void listUpdated();

                ShowsFilter* filter();
                Q_SIGNAL void filterUpdated();

                Enums::SortOrder::Order order();
                void setOrder(Enums::SortOrder::Order order);
                Q_SIGNAL void orderUpdated();

                Q_SIGNAL void error(const QString& header, const QString& body);
                Q_SIGNAL void warn(const QString& text);
            private:
                Library::AddShow* m_addShow;
                ShowSearchList* m_searchResults;
                ShowsList* m_showsList;
                ShowsFilter* m_showsFilter;
                Enums::SortOrder::Order m_order;
                Database::Shows* m_db;

                void load();
            };
        }
    }
}
