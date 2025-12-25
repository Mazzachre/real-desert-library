#include "movie-filter.h"

MovieFilter::MovieFilter() {
}

void MovieFilter::reset() {
}

QString MovieFilter::query() const {
    return u"SELECT movies.id, movies.title, movies.original_title, movies.release_date FROM movies"_qs;
}

QVariantMap MovieFilter::values() const {
    return QVariantMap();
}
