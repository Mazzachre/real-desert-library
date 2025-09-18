#include "episode-list-item.h"

QDebug operator<<(QDebug dbg, const EpisodeListItem& data) {
    dbg.nospace().noquote() << "EpisodeItem:(" << data.id << " S" << data.season << "E" << data.episode << " path:\"" << data.path << "\")";
    return dbg;
}
