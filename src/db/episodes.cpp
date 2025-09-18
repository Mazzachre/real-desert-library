#include "episodes.h"
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileInfo>

Rd::Database::Episodes::Episodes(QObject* parent)
: QObject(parent)
, m_castCrew{new CastCrew} {
    m_create_functions = {
        &Episodes::createEpisode,
        &Episodes::createEpisodeCast,
        &Episodes::createEpisodeCrew
    };
    m_load_functions = {
        &Episodes::loadFile,
        &Episodes::loadCastAndCrew
    };
}

Rd::Database::Episodes::~Episodes() noexcept {
    m_castCrew->deleteLater();
}

QSqlError Rd::Database::Episodes::saveEpisodes(const QList<Episode>& episodes) {
    QSqlDatabase db = QSqlDatabase::database();

    for (auto& episode : episodes) {
        for (auto& fn : m_create_functions) {
            QSqlError err = (this->*fn)(db, episode);
            if (err.type() != QSqlError::NoError) {
                return err;
            }
        }
    }
    return QSqlError();
}

QSqlError Rd::Database::Episodes::linkEpisodes(const QMap<quint32, quint32>& matches) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        return db.lastError();
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO episode_files(file_id, episode_id) VALUES(:fileId, :episodeId)");

    for (auto it = matches.constBegin(); it != matches.constEnd(); ++it) {
        query.bindValue(":fileId", it.key());
        query.bindValue(":episodeId", it.value());

        if (!query.exec()) {
            db.rollback();
            return query.lastError();
        }
    }

    if (!db.commit()) {
        return db.lastError();
    }

    return QSqlError();
}

QSqlError Rd::Database::Episodes::loadEpisodes(const quint32 id, QList<EpisodeListItem>& episodes) {
    QSqlDatabase db = QSqlDatabase::database();

    QSqlError err = load(db, id, episodes);
    if (err.type() != QSqlError::NoError) {
        return err;
    }

    for (auto& episode : episodes) {
        for (auto& fn : m_load_functions) {
            err = (this->*fn)(db, episode);
            if (err.type() != QSqlError::NoError) {
                return err;
            }
        }
    }
    return QSqlError();
}

QSqlError Rd::Database::Episodes::setFavorite(quint32 id, bool favorite) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("UPDATE episodes SET favorite = :favorite WHERE id = :id");
    query.bindValue(":favorite", favorite);
    query.bindValue(":id", id);
    if (!query.exec()) {
        return query.lastError();
    }
    return QSqlError();
}

QSqlError Rd::Database::Episodes::createEpisode(const QSqlDatabase& db, const Episode& episode) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO episodes (id, show_id, season, episode, name, overview, air_date, runtime) VALUES(:id, :show_id, :season, :episode, :name, :overview, :air_date, :runtime)");
    query.bindValue(":id", episode.id);
    query.bindValue(":show_id", episode.showId);
    query.bindValue(":season", static_cast<int>(episode.season));
    query.bindValue(":episode", static_cast<int>(episode.episode));
    query.bindValue(":name", episode.name);
    query.bindValue(":overview", episode.overview);
    query.bindValue(":air_date", QVariant(episode.airDate.toString(Qt::ISODate)));
    query.bindValue(":runtime", static_cast<int>(episode.runtime));
    if (!query.exec()) {
        return query.lastError();
    }
    return QSqlError();
}

QSqlError Rd::Database::Episodes::createEpisodeCast(const QSqlDatabase& db, const Episode& episode) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO episode_cast (episode_id, cast_crew_id, role) VALUES (:episode_id, :cast_crew_id, :role)");

    for (auto it = episode.cast.constBegin(); it != episode.cast.constEnd(); ++it) {
        QSqlError error = m_castCrew->createPerson(it.value());
        if (error.type() != QSqlError::NoError) {
            return error;
        }

        query.bindValue(":episode_id", episode.id);
        query.bindValue(":cast_crew_id", it.value().id);
        query.bindValue(":role", it.key());
        if (!query.exec()) {
            return query.lastError();
        }
    }

    return QSqlError();
}

QSqlError Rd::Database::Episodes::createEpisodeCrew(const QSqlDatabase& db, const Episode& episode) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO episode_cast (episode_id, cast_crew_id, job) VALUES (:episode_id, :cast_crew_id, :job)");

    for (auto it = episode.crew.constBegin(); it != episode.crew.constEnd(); ++it) {
        QSqlError error = m_castCrew->createPerson(it.value());
        if (error.type() != QSqlError::NoError) {
            return error;
        }

        query.bindValue(":episode_id", episode.id);
        query.bindValue(":cast_crew_id", it.value().id);
        query.bindValue(":job", it.key());
        if (!query.exec()) {
            return query.lastError();
        }
    }

    return QSqlError();
}

QSqlError Rd::Database::Episodes::load(const QSqlDatabase& db, const quint32 id, QList<EpisodeListItem>& episodes) {
    QSqlQuery query(db);
    query.prepare("SELECT id, season, episode, name, overview, air_date, runtime, favorite FROM episodes WHERE show_id = :id ORDER BY season, episode ASC");
    query.bindValue(":id", id);
    if (!query.exec()) {
        return query.lastError();
    }
    while (query.next()) {
        EpisodeListItem item;
        item.id = query.value("id").toInt();
        item.season = query.value("season").toInt();
        item.episode = query.value("episode").toInt();
        item.name = query.value("name").toString();
        item.overview = query.value("overview").toString();
        item.airDate = query.value("air_date").toDate();
        item.officialRuntime = query.value("runtime").toInt();
        item.favorite = query.value("favorite").toBool();
        episodes.append(item);
    }

    return QSqlError();
}

QSqlError Rd::Database::Episodes::loadFile(const QSqlDatabase& db, EpisodeListItem& episode) {
    QSqlQuery query(db);
    query.prepare("SELECT path, runtime, selected_subtitle, selected_video, selected_audio, meta, subtitles, video, audio FROM files JOIN episode_files ON episode_files.file_id = files.id WHERE episode_files.episode_id = :episode_id ORDER BY id LIMIT 2");
    query.bindValue(":episode_id", episode.id);
    if (!query.exec()) {
        return query.lastError();
    }

    if (query.next()) {
        episode.path = query.value("path").toString();
        episode.actualRuntime = query.value("runtime").toInt();
        episode.meta = query.value("meta").toMap();

        QVariantMap video;
        QSqlError videoError = getMetaData(query.value("video"), query.value("selected_video"), video);
        if (videoError.type() != QSqlError::NoError) {
            return videoError;
        }
        episode.video = video;

        QVariantMap audio;
        QSqlError audioError = getMetaData(query.value("audio"), query.value("selected_audio"), audio);
        if (audioError.type() != QSqlError::NoError) {
            return audioError;
        }
        episode.audio = audio;

        QVariantMap subtitles;
        QSqlError subtitlesError = getMetaData(query.value("subtitles"), query.value("selected_subtitle"), subtitles);
        if (subtitlesError.type() != QSqlError::NoError) {
            return subtitlesError;
        }
        episode.subtitles = subtitles;
        episode.multiple = query.next();
    }
    return QSqlError();
}

QSqlError Rd::Database::Episodes::loadCastAndCrew(const QSqlDatabase& db, EpisodeListItem& episode) {
    QSqlQuery query(db);
    query.prepare("SELECT cc.id, cc.name, cc.original_name, cc.profile_path, ec.job, ec.role FROM episode_cast ec JOIN cast_crew cc ON cc.id = ec.cast_crew_id WHERE ec.episode_id = :episode_id");
    query.bindValue(":episode_id", episode.id);
    if (!query.exec()) {
        return query.lastError();
    }

    while (query.next()) {
        Person person(query.record());
        if (!query.value("job").isNull()) {
            episode.crew.insert(query.value("job").toString(), person);
        }
        if (!query.value("role").isNull()) {
            episode.cast.insert(query.value("role").toString(), person);
        }
    }

    return QSqlError();
}

QSqlError Rd::Database::Episodes::getMetaData(const QVariant& map, const QVariant& selected, QVariantMap& result) {
    if (selected.metaType() == QMetaType::fromType<QString>() && !selected.toString().isEmpty()) {
        QFileInfo info(selected.toString());
        result["file"] = QVariant(info.completeBaseName());
        result["ext"] = QVariant(info.suffix());
        result["type"] = "external";
        return QSqlError();
    }

    QVariantMap first;
    QJsonParseError parseError;

    QJsonDocument doc = QJsonDocument::fromJson(map.toString().toUtf8(), &parseError);
    if (parseError.error == QJsonParseError::NoError) {
        if (doc.isArray()) {
            QJsonArray values = doc.array();
            bool ok = false;
            int index = selected.toInt(&ok);
            if (ok) {
                if (index < 0 || index > values.count()) {
                    return QSqlError("Meta data parse error", "Selected index out of bounds");
                }
                QJsonValue value = values[index];
                if (value.isObject()) {
                    result = value.toObject().toVariantMap();
                    return QSqlError();
                } else {
                    return QSqlError("Meta data parse error", "Meta data not proper object");
                }
            } else {
                for (const QJsonValue &value : doc.array()) {
                    if (value.isObject()) {
                        QVariantMap map = value.toObject().toVariantMap();
                        if (map["disposition"] == "default") {
                            result = map;
                            return QSqlError();
                        }
                        if (first.isEmpty()) {
                            first = map;
                        }
                    }
                }
                result = first;
            }
        } else {
            return QSqlError("Meta data type error", "Meta data not stored as array");
        }
    } else {
        return QSqlError("Meta data parse error", parseError.errorString());
    }

    return QSqlError();
}
