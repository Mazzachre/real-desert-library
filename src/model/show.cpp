#include "show.h"
#include <QJsonArray>

Show::Show() {
}

Show::Show(const QJsonObject& data) {
    id = data[u"id"_qs].toInt();
    name = data[u"name"].toString();
    originalName = data[u"original_name"].toString();
    tagline = data[u"tagline"].toString();
    overview = data[u"overview"].toString();
    posterPath = data[u"poster_path"].toString();

    QJsonObject externalIds = data[u"external_ids"].toObject();
    imdb = externalIds[u"imdb_id"].toString();

    QJsonArray langs = data[u"languages"].toArray();
    for (const QJsonValue& val : langs) {
        languages.append(val.toString());
    }

    QJsonArray countries = data[u"origin_country"].toArray();
    for (const QJsonValue& val : countries) {
        origin.append(val.toString());
    }

    QJsonArray gens = data[u"genres"].toArray();
    for (const QJsonValue& val : gens) {
        QJsonObject genre = val.toObject();
        genres.insert(genre[u"id"].toInt(), genre[u"name"].toString());
    }

    QJsonObject credits = data[u"credits"].toObject();
    QJsonArray stars = credits[u"cast"].toArray();
    for (const QJsonValue& val : stars) {
        QJsonObject data = val.toObject();

        Person person(data);
        QString role = data[u"character"].toString();
        cast.insert(role, person);
    }

    QJsonArray workers = credits[u"crew"].toArray();
    for (const QJsonValue& val : workers) {
        QJsonObject data = val.toObject();

        Person person(data);
        QString job = data[u"department"].toString() + u":"_qs + data[u"job"].toString();
        crew.insert(job, person);
    }
}

QDebug operator<<(QDebug dbg, const Show& data) {
    dbg.nospace().noquote() << "Show:(" << data.id << " " <<  data.name << " cast<" << data.cast << "> crew<" << data.crew << ">)";
    return dbg;
}
