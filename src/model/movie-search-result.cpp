#include "movie-search-result.h"

#include <QJsonObject>
#include <QDebug>

MovieSearchResult::MovieSearchResult() {
}

MovieSearchResult::MovieSearchResult(const QJsonObject& data) {
    id = data["id"].toInt();
    title = data["title"].toString();
    originalLanguage = data["original_language"].toString();
    originalTitle = data["original_title"].toString();
    overview = data["overview"].toString();
    popularity = data["popularity"].toDouble();
    posterPath = data["poster_path"].toString();
    releaseDate = QDate::fromString(data["release_date"].toString().trimmed(), Qt::ISODate);
}

QDebug operator <<(QDebug dbg, const MovieSearchResult& result) {
    dbg.nospace().noquote() << "MovieSearchResult:(" << result.id << " " <<  result.title << " " << result.releaseDate << " " << result.originalTitle << ")";
    return dbg;
}
