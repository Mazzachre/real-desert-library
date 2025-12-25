#include "shows.h"
#include <QSqlQuery>

Rd::Database::Shows::Shows(QObject* parent)
: QObject(parent)
, m_castCrew{new CastCrew} {
    m_create_functions = {
        &Shows::createShow,
        &Shows::createShowCast,
        &Shows::createShowCrew
    };
    m_load_functions = {
        &Shows::loadShow,
        &Shows::loadGenres,
        &Shows::loadTags,
        &Shows::loadShowCastCrew,
        &Shows::loadHasExtras
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
    if (!query.exec()) return query.lastError();
    exists = query.next();
    return QSqlError();
}

QSqlError Rd::Database::Shows::findShows(const ShowFilter& filter, Rd::Enums::SortOrder::Order order, QList<ShowListItem>& result) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    QString sql = filter.query();

    switch(order) {
        case Enums::SortOrder::Order::ReleaseAsc:
            sql += " ORDER BY first_air_date ASC";
            break;
        case Enums::SortOrder::Order::ReleaseDesc:
            sql += " ORDER BY first_air_date DESC";
            break;
        case Enums::SortOrder::Order::TitleAsc:
            sql += " ORDER BY shows.name ASC";
            break;
        case Enums::SortOrder::Order::TitleDesc:
            sql += " ORDER BY shows.name DESC";
            break;
    }

    query.prepare(sql);
    const QVariantMap values = filter.values();

    qDebug() << sql << values;

    for (auto it = values.constBegin(); it != values.constEnd(); ++it) {
        query.bindValue(it.key(), it.value());
    }

    if (!query.exec()) return query.lastError();

    while (query.next()) {
        result.append(ShowListItem(query.record()));
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::findByEpisode(const quint32 id, quint32& result) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT show_id FROM episodes WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) return query.lastError();
    if (!query.next()) return QSqlError("", "Show id not found", QSqlError::UnknownError);

    result = query.value("show_id").toUInt();

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
    query.prepare("SELECT DISTINCT g.id, g.name FROM genres g JOIN genre_links gl ON gl.genre_id = g.id JOIN shows s ON s.imdb = gl.imdb_id;");
    if (!query.exec()) return query.lastError();

    while (query.next()) {
        genres.insert(query.value("id").toInt(), query.value("name").toString());
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::unlinkGenre(const QString& imdbId, quint32 genre) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        return db.lastError();
    }

    QString source;
    QSqlError srcErr = getLinkSource(db, imdbId, genre, source);
    if (srcErr.type() != QSqlError::NoError) {
        db.rollback();
        return srcErr;
    }

    QSqlError delErr = deleteLink(db, imdbId, genre);
    if (delErr.type() != QSqlError::NoError) {
        db.rollback();
        return delErr;
    }

    if (source == u"IMDB"_qs) {
        QSqlError unlErr = saveUnlink(db, imdbId, genre);
        if (unlErr.type() != QSqlError::NoError) {
            db.rollback();
            return unlErr;
        }
    }

    if (!db.commit()) {
        return db.lastError();
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::linkGenre(const QString& imdbId, quint32 genre) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("INSERT INTO genre_links(imdb_id, genre_id, source) VALUES(:imdb, :genre, 'USER')");
    query.bindValue(":imdb", QVariant(imdbId));
    query.bindValue(":genre", genre);
    query.exec();
    return query.lastError();
}

QSqlError Rd::Database::Shows::getTags(QMap<quint16, QString>& tags) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT DISTINCT tags.id, tags.name FROM tags JOIN show_tags ON tags.id = show_tags.tag_id");
    if (!query.exec()) return query.lastError();

    while (query.next()) {
        tags.insert(query.value("id").toInt(), query.value("name").toString());
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::linkTag(const quint32 id, quint32 tag) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO show_tags(show_id, tag_id) VALUES(:showId, :tagId)");
    query.bindValue(":showId", id);
    query.bindValue(":tagId", tag);
    query.exec();
    return query.lastError();
}

QSqlError Rd::Database::Shows::unlinkTag(const quint32 id, quint32 tag) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("DELETE FROM show_tags WHERE show_id = :showId AND tag_id = :tagId");
    query.bindValue(":showId", id);
    query.bindValue(":tagId", tag);
    query.exec();
    return query.lastError();
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

QSqlError Rd::Database::Shows::createShowCast(const QSqlDatabase& db, const Show& show) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO show_cast (show_id, cast_crew_id, role) VALUES (:show_id, :cast_crew_id, :role)");

    for (auto& it : show.cast) {
        QSqlError error = m_castCrew->createPerson(it.person);
        if (error.type() != QSqlError::NoError) return error;

        query.bindValue(":show_id", show.id);
        query.bindValue(":cast_crew_id", it.person.id);
        query.bindValue(":role", it.role);
        if (!query.exec()) return query.lastError();
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::createShowCrew(const QSqlDatabase& db, const Show& show) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO show_cast (show_id, cast_crew_id, job) VALUES (:show_id, :cast_crew_id, :job)");

    for (auto& it : show.crew) {
        QSqlError error = m_castCrew->createPerson(it.person);
        if (error.type() != QSqlError::NoError) return error;

        query.bindValue(":show_id", show.id);
        query.bindValue(":cast_crew_id", it.person.id);
        query.bindValue(":job", it.job);
        if (!query.exec()) return query.lastError();
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::loadShow(const QSqlDatabase& db, const quint32 id, Show& show) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM shows WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) return query.lastError();
    if (!query.next()) return QSqlError("", "Show not found", QSqlError::UnknownError);

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
    query.prepare("SELECT g.id, g.source, g.name FROM genre_links sg JOIN genres g ON g.id = sg.genre_id WHERE sg.imdb_id = :imdb_id ORDER BY name ASC");
    query.bindValue(":imdb_id", show.imdb);
    if (!query.exec()) return query.lastError();

    while(query.next()) {
        show.genres.append(Genre(query.record()));
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::loadTags(const QSqlDatabase& db, const quint32 id, Show& show) {
    QSqlQuery query(db);
    query.prepare("SELECT t.id, t.name FROM show_tags st JOIN tags t ON t.id = st.tag_id WHERE st.show_id = :show_id ORDER BY name ASC");
    query.bindValue(":show_id", id);
    if (!query.exec()) return query.lastError();

    while (query.next()) {
        QVariantMap item;
        item.insert("id", query.value("id"));
        item.insert("name", query.value("name"));
        show.tags.append(item);
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::loadShowCastCrew(const QSqlDatabase& db, const quint32 id, Show& show) {
    QSqlQuery query(db);
    query.prepare("SELECT cc.id, cc.name, cc.original_name, cc.profile_path, sc.job, sc.role FROM show_cast sc JOIN cast_crew cc ON cc.id = sc.cast_crew_id WHERE sc.show_id = :show_id ORDER BY cc.id");
    query.bindValue(":show_id", id);
    if (!query.exec()) return query.lastError();

    while (query.next()) {
        if (!query.value("job").isNull()) {
            show.crew.append(Crew(query.record()));
        }
        if (!query.value("role").isNull()) {
            show.cast.append(Cast(query.record()));
        }
    }

    std::sort(show.cast.begin(), show.cast.end());

    return QSqlError();
}

QSqlError Rd::Database::Shows::loadHasExtras(const QSqlDatabase& db, const quint32 showId, Show& show) {
    QSqlQuery query(db);
    query.prepare("SELECT EXISTS(SELECT 1 FROM extras WHERE show_id = :showId)");
    query.bindValue(":showId", showId);

    if (!query.exec()) return query.lastError();

    if (query.next()) {
        show.hasExtras = query.value(0).toBool();
    } else {
        show.hasExtras = false;
    }

    return QSqlError();
}

QSqlError Rd::Database::Shows::getLinkSource(const QSqlDatabase& db, const QString& imdbId, quint32 genre, QString& source) {
    QSqlQuery query(db);
    query.prepare("SELECT source FROM genre_links WHERE imdb_id = :imdbId AND genre_id = :genreId");
    query.bindValue(":imdbId", QVariant(imdbId));
    query.bindValue(":genreId", genre);

    if (!query.exec()) {
        return query.lastError();
    }
    if (!query.next()) {
        return QSqlError("Not found", "No such genre link", QSqlError::NoError);
    }
    source = query.value("source").toString();
    return QSqlError();
}

QSqlError Rd::Database::Shows::deleteLink(const QSqlDatabase& db, const QString& imdbId, quint32 genre) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM genre_links WHERE imdb_id = :imdbId AND genre_id = :genreId");
    query.bindValue(":imdbId", QVariant(imdbId));
    query.bindValue(":genreId", genre);
    query.exec();
    return query.lastError();
}

QSqlError Rd::Database::Shows::saveUnlink(const QSqlDatabase& db, const QString& imdbId, quint32 genre) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO genre_unlinks (imdb_id, genre_id) VALUES (:imdbId, :genreId)");
    query.bindValue(":imdbId", QVariant(imdbId));
    query.bindValue(":genreId", genre);
    query.exec();
    return query.lastError();
}
