#include "episode.h"
#include <QJsonArray>

Episode::Episode() {
}

Episode::Episode(const QJsonObject& data) {
    id = data[u"id"_qs].toInt();
    showId = data[u"show_id"_qs].toInt();
    season = data[u"season_number"_qs].toInt();
    episode = data[u"episode_number"_qs].toInt();
    name = data[u"name"].toString();
    overview = data[u"overview"].toString();
    runtime = data[u"runtime"_qs].toInt();
    airDate = QDate::fromString(data[u"air_date"].toString().trimmed(), Qt::ISODate);

    QJsonArray stars = data[u"guest_stars"].toArray();
    for (const QJsonValue& val : stars) {
        QJsonObject data = val.toObject();

        Person person(data);
        QString role = data[u"character"].toString();
        cast.insert(role, person);
    }

    QJsonArray workers = data[u"crew"].toArray();
    for (const QJsonValue& val : workers) {
        QJsonObject data = val.toObject();

        Person person(data);
        QString job = data[u"department"].toString() + u":"_qs + data[u"job"].toString();
        crew.insert(job, person);
    }
}

QDebug operator<<(QDebug dbg, const Episode& data) {
    dbg.nospace().noquote() << "Episode:(" << data.season << " " << data.episode << " " <<  data.name << " cast<: >" << data.cast << "> crew<" << data.crew << ">)";
    return dbg;
}
