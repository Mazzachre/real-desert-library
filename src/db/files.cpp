#include "files.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>

Rd::Database::Files::Files(QObject* parent)
: QObject(parent) {
}

Rd::Database::Files::~Files() noexcept {
}

QSqlError Rd::Database::Files::exists(const QString& path, quint32& id) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    query.prepare("SELECT id FROM files WHERE path = :path LIMIT 1");
    query.bindValue(":path", QVariant(path));

    if (!query.exec()) {
        return query.lastError();
    }
    if(query.next()) {
        id = query.value("id").toInt();
    } else {
        id = 0;
    }
    return QSqlError();
}

QSqlError Rd::Database::Files::get(const quint32& id, File& file) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    query.prepare("SELECT * FROM files WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        return query.lastError();
    }
    if (!query.next()) {
        return QSqlError("", "File not found", QSqlError::UnknownError);
    }

    file.id = query.value("id").toUInt();
    file.path = query.value("path").toString();
    file.created = QDate::fromString(query.value("created").toString(), Qt::ISODate);
    file.selectedSubtitle = query.value("selected_subtitle").toString();
    file.selectedVideo = query.value("selected_video").toUInt();
    file.selectedAudio = query.value("selected_audio").toUInt();
    file.meta = QJsonDocument::fromJson(query.value("meta").toString().toUtf8()).toVariant().toMap();
    file.runtime = query.value("runtime").toUInt();
    file.subtitles = QJsonDocument::fromJson(query.value("subtitles").toString().toUtf8()).toVariant().toList();
    file.video = QJsonDocument::fromJson(query.value("video").toString().toUtf8()).toVariant().toList();
    file.audio = QJsonDocument::fromJson(query.value("audio").toString().toUtf8()).toVariant().toList();

    return QSqlError();
}

QSqlError Rd::Database::Files::create(File& file) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("INSERT INTO files(path, created, meta, runtime, subtitles, video, audio) VALUES(:path, :created, :meta, :runtime, :subtitles, :video, :audio)");

    query.bindValue(":path", file.path);
    query.bindValue(":created", QVariant(QDate::currentDate().toString(Qt::ISODate)));
    query.bindValue(":meta", QString::fromUtf8(QJsonDocument::fromVariant(file.meta).toJson(QJsonDocument::Compact)));
    query.bindValue(":runtime", static_cast<int>(file.runtime));
    query.bindValue(":subtitles", QString::fromUtf8(QJsonDocument::fromVariant(file.subtitles).toJson(QJsonDocument::Compact)));
    query.bindValue(":video", QString::fromUtf8(QJsonDocument::fromVariant(file.video).toJson(QJsonDocument::Compact)));
    query.bindValue(":audio", QString::fromUtf8(QJsonDocument::fromVariant(file.audio).toJson(QJsonDocument::Compact)));

    if (!query.exec()) {
        return query.lastError();
    } else {
        file.id = query.lastInsertId().toUInt();
    }

    return QSqlError();
}

QSqlError Rd::Database::Files::getLinks(const quint32& id, QVariantMap& links) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT 'movie' AS type, movie_id AS linked_id FROM movie_files WHERE file_id = :file_id
        UNION ALL
        SELECT 'episode' AS type, episode_id AS linked_id FROM episode_files WHERE file_id = :file_id
        UNION ALL
        SELECT 'extra_movie' AS type, movie_id AS linked_id FROM extras WHERE file_id = :file_id AND movie_id IS NOT NULL
        UNION ALL
        SELECT 'extra_episode' AS type, episode_id AS linked_id FROM extras WHERE file_id = :file_id AND episode_id IS NOT NULL
        UNION ALL
        SELECT 'extra_show' AS type, show_id AS linked_id FROM extras WHERE file_id = :file_id AND show_id IS NOT NULL
    )");
    query.bindValue(":file_id", id);

    if (query.next()) {
        links["type"] = QVariant(query.value("type").toString());
        links["id"] = query.value("linked_id").toInt();

        if (query.next()) {
            return QSqlError("", "File has multiple links!", QSqlError::UnknownError);
        }
    }

    return QSqlError();
}
