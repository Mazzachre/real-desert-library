#pragma once

#include <QObject>
#include "show-search-list.h"
#include "shows-list.h"
#include "shows-filter.h"
#include "../../lib/add-show.h"
#include "../sort-order-wrapper.h"
#include "../../model/sort-order.h"
#include "../../db/shows.h"

namespace Rd {
    namespace Ui {
        namespace Shows {
            class Shows : public QObject {
                Q_OBJECT
                Q_PROPERTY(QAbstractListModel* searchResults READ searchResults NOTIFY searchResultsUpdated)
                Q_PROPERTY(QAbstractListModel* list READ list NOTIFY listUpdated)
                Q_PROPERTY(ShowsFilter* filter READ filter NOTIFY filterUpdated)
                Q_PROPERTY(Rd::Ui::SortOrderWrapper::WrappedSortOrder order READ order WRITE setOrder NOTIFY orderUpdated)
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

                Rd::Ui::SortOrderWrapper::WrappedSortOrder order();
                void setOrder(Rd::Ui::SortOrderWrapper::WrappedSortOrder order);
                Q_SIGNAL void orderUpdated();

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                Rd::Library::AddShow* m_addShow;
                ShowSearchList* m_searchResults;
                ShowsList* m_showsList;
                ShowsFilter* m_showsFilter;
                Rd::Ui::SortOrderWrapper::WrappedSortOrder m_order;
                Rd::Database::Shows* m_db;

                void load();
            };
        }
    }
}
