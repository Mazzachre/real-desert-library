#include "show-list-item.h"

ShowListItem::ShowListItem(){
}

ShowListItem::ShowListItem(const QSqlRecord& record) {
    id = record.value("id").toInt();
    name = record.value("name").toString();
    originalName = record.value("original_name").toString();
    firstAirDate = QDate::fromString(record.value("first_air_date").toString(), Qt::ISODate);
    episodeCount = record.value("episode_count").toUInt();
    officialEpisodeCount = record.value("official_episode_count").toUInt();
}

QDebug operator<<(QDebug dbg, const ShowListItem& data) {
    dbg.nospace().noquote() << "Show:(" << data.id << " " <<  data.name << " episodes " << data.officialEpisodeCount << " files " << data.episodeCount << ")";
    return dbg;
}
