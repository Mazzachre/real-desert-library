#include "movie-list-item.h"

MovieListItem::MovieListItem() {
}

MovieListItem::MovieListItem(const QSqlRecord& record) {
    id = record.value("id").toInt();
    title = record.value("title").toString();
    originalTitle = record.value("original_title").toString();
    releaseDate = QDate::fromString(record.value("release_date").toString(), Qt::ISODate);
}

QDebug operator<<(QDebug dbg, const MovieListItem& data) {
    dbg.nospace().noquote() << "Show:(" << data.id << " " <<  data.title << ")";
    return dbg;
}
