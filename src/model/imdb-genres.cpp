#include "imdb-genres.h"

QDebug operator<<(QDebug dbg, const ImdbGenres& data) {
    dbg.nospace().noquote() << "ImdbGenres:(" << data.id << " <" << data.genres << ">)";
    return dbg;
}
