#include "movies.h"
#include <QSqlQuery>

Rd::Database::Movies::Movies(QObject* parent)
: QObject(parent) {
}

Rd::Database::Movies::~Movies() noexcept {
}

QSqlError Rd::Database::Movies::exists(const quint32 id, bool& exists) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM movies WHERE id = :id LIMIT 1");
    query.bindValue(":id", id);
    if (!query.exec()) return query.lastError();
    exists = query.next();
    return QSqlError();
}


QSqlError Rd::Database::Movies::findMovies(const MovieFilter& filter, Rd::Enums::SortOrder::Order order, QList<MovieListItem>& result) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    QString sql = filter.query();

    switch(order) {
        case Enums::SortOrder::Order::ReleaseAsc:
            sql += " ORDER BY movies.release_date ASC";
            break;
        case Enums::SortOrder::Order::ReleaseDesc:
            sql += " ORDER BY movies.release_date DESC";
            break;
        case Enums::SortOrder::Order::TitleAsc:
            sql += " ORDER BY movies.title ASC";
            break;
        case Enums::SortOrder::Order::TitleDesc:
            sql += " ORDER BY movies.title DESC";
            break;
    }

    query.prepare(sql);
    const QVariantMap values = filter.values();

    qDebug() << sql << values;

    for (auto it = values.constBegin(); it != values.constEnd(); ++it) {
        query.bindValue(it.key(), it.value());
    }

    if (!query.exec()) return query.lastError();

    while (query.next()) {
        result.append(MovieListItem(query.record()));
    }

    return QSqlError();
}
