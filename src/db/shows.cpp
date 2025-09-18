#include "shows.h"
#include <QSqlDatabase>
#include <QSqlQuery>

Rd::Database::Shows::Shows(QObject* parent)
: QObject(parent)
, m_castCrew{new CastCrew} {
    m_create_functions = {
        &Shows::createShow,
        &Shows::createGenres,
        &Shows::createTags,
        &Shows::createShowCast,
        &Shows::createShowCrew
    };
    m_load_functions = {
        &Shows::loadShow,
        &Shows::loadGenres,
        &Shows::loadTags,
        &Shows::loadShowCastCrew
    };
}

Rd::Database::Shows::~Shows() noexcept {
    m_castCrew->deleteLater();
}

QSqlError Rd::Database::Shows::exists(quint32 id, bool& exists) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM shows WHERE id = :id LIMIT 1");
    query.bindValue(":id", id);
    if (!query.exec()) {
        return query.lastError();
    }
    exists = query.next();
    return QSqlError();
}

QSqlError Rd::Database::Shows::findShows(const ShowFilter& filter, SortOrder order, QList<ShowListItem>& result) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    QString sql = filter.query();

    switch(order) {
        case ReleaseAsc:
            sql += " ORDER BY first_air_date ASC";
            break;
        case ReleaseDesc:
            sql += " ORDER BY first_air_date DESC";
            break;
        case TitleAsc:
            sql += " ORDER BY shows.name ASC";
            break;
        case TitleDesc:
            sql += " ORDER BY shows.name DESC";
            break;
    }

    query.prepare(sql);
    const QVariantMap values = filter.values();

    qDebug() << sql << values;

    for (auto it = values.constBegin(); it != values.constEnd(); ++it) {
        query.bindValue(it.key(), it.value());
    }

    if(!query.exec()) {
        return query.lastError();
    }

    while (query.next()) {
        result.append(ShowListItem(query.record()));
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::getShow(const quint32 id, Show& result) {
    QSqlDatabase db = QSqlDatabase::database();

    for (auto fn : m_load_functions) {
        QSqlError err = (this->*fn)(db, id, result);
        if (err.type() != QSqlError::NoError) {
            return err;
        }
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::saveShow(const Show& show) {
    QSqlDatabase db = QSqlDatabase::database();

    for (auto fn : m_create_functions) {
        QSqlError err = (this->*fn)(db, show);
        if (err.type() != QSqlError::NoError) {
            db.rollback();
            return err;
        }
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::getGenres(QMap<quint16, QString>& genres) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT DISTINCT genres.id, genres.name FROM genres JOIN show_genres ON genres.id = show_genres.genre_id");
    if (!query.exec()) {
        return query.lastError();
    }

    while (query.next()) {
        genres.insert(query.value("id").toInt(), query.value("name").toString());
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::getTags(QMap<quint16, QString>& tags) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT DISTINCT tags.id, tags.name FROM tags JOIN show_tags ON tags.id = show_tags.tag_id");
    if (!query.exec()) {
        return query.lastError();
    }

    while (query.next()) {
        tags.insert(query.value("id").toInt(), query.value("name").toString());
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::createShow(const QSqlDatabase& db, const Show& show) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO shows (id, imdb, name, original_name, origin, tagline, overview, languages, poster_path) VALUES (:id, :imdb, :name, :original_name, :origin, :tagline, :overview, :languages, :poster_path)");
    query.bindValue(":id", show.id);
    query.bindValue(":imdb", show.imdb);
    query.bindValue(":name", show.name);
    query.bindValue(":original_name", show.originalName);
    query.bindValue(":origin", show.origin.join(','));
    query.bindValue(":tagline", show.tagline);
    query.bindValue(":overview", show.overview);
    query.bindValue(":languages", show.languages.join(','));
    query.bindValue(":poster_path", show.posterPath);
    query.exec();
    return query.lastError();
}

QSqlError Rd::Database::Shows::createGenres(const QSqlDatabase& db, const Show& show) {
    QSqlQuery create(db);
    create.prepare("INSERT OR IGNORE INTO genres (id, name) VALUES (:id, :name)");
    QSqlQuery link(db);
    link.prepare("INSERT INTO show_genres (show_id, genre_id) VALUES (:show_id, :genre_id)");

    for (auto it = show.genres.constBegin(); it != show.genres.constEnd(); ++it) {
        create.bindValue(":id", it.key());
        create.bindValue(":name", it.value());
        link.bindValue(":show_id", show.id);
        link.bindValue(":genre_id", it.key());

        if (!create.exec()) {
            return create.lastError();
        }
        if (!link.exec()) {
            return link.lastError();
        }
    }
    return QSqlError();
}

QSqlError Rd::Database::Shows::createTags(const QSqlDatabase& db, const Show& show) {
    QSqlQuery create(db);
    create.prepare("INSERT OR IGNORE INTO tags (id, name) VALUES (:id, :name)");
    QSqlQuery link(db);
    link.prepare("INSERT INTO show_tags (show_id, genre_id) VALUES (:show_id, :genre_id)");

    for (auto it = show.tags.constBegin(); it != show.tags.constEnd(); ++it) {
        create.bindValue(":id", it.key());
        create.bindValue(":name", it.value());
        link.bindValue(":show_id", show.id);
        link.bindValue(":genre_id", it.key());

        if (!create.exec()) {
            return create.lastError();
        }
        if (!link.exec()) {
            return link.lastError();
        }
    }
    return QSqlError();
}

QSqlError Rd::Database::Shows::createShowCast(const QSqlDatabase& db, const Show& show) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO show_cast (show_id, cast_crew_id, role) VALUES (:show_id, :cast_crew_id, :role)");

    for (auto it = show.cast.constBegin(); it != show.cast.constEnd(); ++it) {
        QSqlError error = m_castCrew->createPerson(it.value());
        if (error.type() != QSqlError::NoError) {
            return error;
        }

        query.bindValue(":show_id", show.id);
        query.bindValue(":cast_crew_id", it.value().id);
        query.bindValue(":role", it.key());
        if (!query.exec()) {
            return query.lastError();
        }
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::createShowCrew(const QSqlDatabase& db, const Show& show) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO show_cast (show_id, cast_crew_id, job) VALUES (:show_id, :cast_crew_id, :job)");

    for (auto it = show.crew.constBegin(); it != show.crew.constEnd(); ++it) {
        QSqlError error = m_castCrew->createPerson(it.value());
        if (error.type() != QSqlError::NoError) {
            return error;
        }

        query.bindValue(":show_id", show.id);
        query.bindValue(":cast_crew_id", it.value().id);
        query.bindValue(":job", it.key());
        if (!query.exec()) {
            return query.lastError();
        }
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::loadShow(const QSqlDatabase& db, const quint32 id, Show& show) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM shows WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        return query.lastError();
    }
    if (!query.next()) {
        return QSqlError("", "Show not found", QSqlError::UnknownError);
    }

    show.id = query.value("id").toInt();
    show.imdb = query.value("imdb").toString();
    show.name = query.value("name").toString();
    show.originalName = query.value("original_name").toString();
    show.origin = query.value("origin").toString().split(',');
    show.tagline = query.value("tagline").toString();
    show.overview = query.value("overview").toString();
    show.languages = query.value("languages").toString().split(',');
    show.posterPath = query.value("poster_path").toString();

    return QSqlError();
}

QSqlError Rd::Database::Shows::loadGenres(const QSqlDatabase& db, const quint32 id, Show& show) {
    QSqlQuery query(db);
    query.prepare("SELECT g.id, g.name FROM show_genres sg JOIN genres g ON g.id = sg.genre_id WHERE sg.show_id = :show_id");
    query.bindValue(":show_id", id);
    if (!query.exec()) {
        return query.lastError();
    }

    while(query.next()) {
        show.genres.insert(query.value("id").toInt(), query.value("name").toString());
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::loadTags(const QSqlDatabase& db, const quint32 id, Show& show) {
    QSqlQuery query(db);
    query.prepare("SELECT t.id, t.name FROM show_tags st JOIN tags t ON t.id = st.tag_id WHERE st.show_id = :show_id");
    query.bindValue(":show_id", id);
    if (!query.exec()) {
        return query.lastError();
    }

    while (query.next()) {
        show.tags.insert(query.value("id").toInt(), query.value("name").toString());
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::loadShowCastCrew(const QSqlDatabase& db, const quint32 id, Show& show) {
    QSqlQuery query(db);
    query.prepare("SELECT cc.id, cc.name, cc.original_name, cc.profile_path, sc.job, sc.role FROM show_cast sc JOIN cast_crew cc ON cc.id = sc.cast_crew_id WHERE sc.show_id = :show_id");
    query.bindValue(":show_id", id);
    if (!query.exec()) {
        return query.lastError();
    }

    while (query.next()) {
        Person person(query.record());
        if (!query.value("job").isNull()) {
            show.crew.insert(query.value("job").toString(), person);
        }
        if (!query.value("role").isNull()) {
            show.cast.insert(query.value("role").toString(), person);
        }
    }

    return QSqlError();
}
