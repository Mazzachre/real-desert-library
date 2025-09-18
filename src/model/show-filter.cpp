#include "show-filter.h"
#include <QDate>

ShowFilter::ShowFilter() {
    newerThan = 0;
    olderThan = 0;
}

QString ShowFilter::query() const {
    QString select = {
        "SELECT DISTINCT shows.id, shows.name, shows.original_name, ep1.air_date AS first_air_date, (SELECT COUNT(*) FROM episodes e JOIN episode_files ef ON e.id = ef.episode_id WHERE e.show_id = shows.id) AS episode_count, (SELECT COUNT(*) FROM episodes WHERE show_id = shows.id) AS official_episode_count FROM shows LEFT JOIN episodes ep1 ON ep1.show_id = shows.id AND ep1.season = 1 AND ep1.episode = 1"
    };

    if (!genres.isEmpty()) {
        select += " JOIN show_genres sg ON sg.show_id = shows.id";
    }

    if (!tags.isEmpty()) {
        select += " JOIN show_tags st ON st.show_id = shows.id";
    }

    QStringList query;
    QStringList having;

    if (newerThan > 0) {
        query << "ep1.air_date >= :newerThan";
    }
    if (olderThan > 0) {
        query << "ep1.air_date <= :olderThan";
    }

    if (!genres.isEmpty()) {
        QStringList keys;
        for (int i = 0; i < genres.count(); ++i) {
            keys << QString(":genre%1").arg(i);
        }
        query << QString("sg.genre_id IN (%1)").arg(keys.join(','));
        having << "COUNT(DISTINCT sg.genre_id) = :genreCount";
    }

    if (!tags.isEmpty()) {
        QStringList keys;
        for (int i = 0; i < tags.count(); ++i) {
            keys << QString(":tag%1").arg(i);
        }
        query << QString("st.tag_id IN (%1)").arg(keys.join(','));
        having << "COUNT(DISTINCT st.tag_id) = :tagCount";
    }

    if (!query.isEmpty()) {
        select += " WHERE " + query.join(" AND ");
    }

    if (!having.isEmpty()) {
        select += " GROUP BY shows.id HAVING " + having.join(" AND ");
    }

    return select;
}

QVariantMap ShowFilter::values() const {
    QVariantMap retval;

    if (newerThan > 0) {
        retval.insert(":newerThan", QDate(newerThan, 1, 1));
    }
    if (olderThan > 0) {
        retval.insert(":olderThan", QDate(olderThan, 1, 1));
    }

    for (int i = 0; i < genres.count(); ++i) {
        QString key = QString(":genre%1").arg(i);
        retval.insert(key, genres[i]);
    }
    if (!genres.isEmpty()) {
        retval.insert(":genreCount", genres.count());
    }

    for (int i = 0; i < tags.count(); ++i) {
        QString key = QString(":tags%1").arg(i);
        retval.insert(key, tags[i]);
    }
    if (!tags.isEmpty()) {
        retval.insert(":tagCount", tags.count());
    }

    return retval;
}


    /*

if (!filter.origin.isEmpty())
{
    whereClauses << "shows.origin LIKE :origin";
    bindValues[":origin"] = "%" + filter.origin + "%";
}

LEFT JOIN show_cast sc ON sc.show_id = shows.id
LEFT JOIN cast_crew cc ON cc.id = sc.cast_crew_id

if (!filter.people.isEmpty())
{
    QStringList orConditions;
    for (int i = 0; i < filter.people.size(); ++i)
    {
        QString key = QString(":person%1").arg(i);
        orConditions << QString("cc.name LIKE %1 OR cc.original_name LIKE %1").arg(key);
        bindValues[key] = "%" + filter.people[i] + "%";
    }
    whereClauses << "(" + orConditions.join(" OR ") + ")";
}
*/
