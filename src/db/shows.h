#pragma once

#include <QObject>
#include <QList>
#include <QSqlError>
#include <QSqlDatabase>
#include "cast-crew.h"
#include "../model/show.h"
#include "../model/show-list-item.h"
#include "../model/sort-order.h"
#include "../model/show-filter.h"

namespace Rd {
    namespace Database {
        class Shows : public QObject {
            Q_OBJECT
        public:
            explicit Shows(QObject* parent = nullptr);
            ~Shows() noexcept;

            QSqlError exists(const quint32 id, bool& exists);
            QSqlError findShows(const ShowFilter& filter, SortOrder order, QList<ShowListItem>& result);
            QSqlError getShow(const quint32 id, Show& result);
            QSqlError saveShow(const Show& show);

            QSqlError getGenres(QMap<quint16, QString>& genres);
            QSqlError getTags(QMap<quint16, QString>& tags);
        private:
            CastCrew* m_castCrew;

            using CreateFn = QSqlError (Shows::*)(const QSqlDatabase&, const Show&);
            std::vector<CreateFn> m_create_functions;

            QSqlError createShow(const QSqlDatabase& db, const Show& show);
            QSqlError createGenres(const QSqlDatabase& db, const Show& show);
            QSqlError createTags(const QSqlDatabase& db, const Show& show);
            QSqlError createShowCast(const QSqlDatabase& db, const Show& show);
            QSqlError createShowCrew(const QSqlDatabase& db, const Show& show);

            using LoadFn = QSqlError (Shows::*)(const QSqlDatabase&, const quint32, Show&);
            std::vector<LoadFn> m_load_functions;

            QSqlError loadShow(const QSqlDatabase& db, const quint32 id, Show& show);
            QSqlError loadGenres(const QSqlDatabase& db, const quint32 id, Show& show);
            QSqlError loadTags(const QSqlDatabase& db, const quint32 id, Show& show);
            QSqlError loadShowCastCrew(const QSqlDatabase& db, const quint32 id, Show& show);
        };
    }
}
