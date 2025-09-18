#include "show-search-result.h"

#include <QJsonObject>
#include <QDebug>

ShowSearchResult::ShowSearchResult() {
}

ShowSearchResult::ShowSearchResult(const QJsonObject& data) {
    id = data[u"id"].toInt();
    name = data[u"name"].toString();
    originalLanguage = data[u"original_language"].toString();
    originalName = data[u"original_name"].toString();
    overview = data[u"overview"].toString();
    popularity = data[u"popularity"].toDouble();
    posterPath = data[u"poster_path"].toString();
    firstAirDate = QDate::fromString(data[u"first_air_date"].toString().trimmed(), Qt::ISODate);
}

QDebug operator<<(QDebug dbg, const ShowSearchResult& data) {
    dbg.nospace().noquote() << "ShowSearchResult:(" << data.id << " " <<  data.name << " " << data.firstAirDate << ")";
    return dbg;
}
