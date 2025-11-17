#include "extras.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

Rd::Database::Extras::Extras(QObject* parent)
: QObject(parent)
, m_playbacks{new Playbacks} {
}

Rd::Database::Extras::~Extras() noexcept {
    m_playbacks->deleteLater();
}

QSqlError Rd::Database::Extras::saveExtras(const QList<Extra>& items) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        return db.lastError();
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO extras(file_id, name, show_id, episode_id, movie_id) VALUES(:file_id, :name, :show_id, :episode_id, :movie_id)");

    for (auto& item : items) {
        query.bindValue(":file_id", item.fileId);
        query.bindValue(":name", item.name);
        query.bindValue(":show_id", QVariant(QMetaType::fromType<quint32>()));
        query.bindValue(":episode_id", QVariant(QMetaType::fromType<quint32>()));
        query.bindValue(":movie_id", QVariant(QMetaType::fromType<quint32>()));
        query.bindValue(u":"_qs+Enums::ExtrasType::getColumn(item.type), item.id);

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

QSqlError Rd::Database::Extras::updateName(quint32 id, const QString& name) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("UPDATE extras SET name = :name WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":name", QVariant(name));
    if (!query.exec()) {
        return query.lastError();
    }
    return QSqlError();
}

QSqlError Rd::Database::Extras::loadExtras(Enums::ExtrasType::Type type, quint32 id, QList<ExtraListItem>& result) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    QString sql = QString("SELECT e.id, e.name, f.id AS file_id, f.path, f.runtime FROM extras e JOIN files f ON e.file_id = f.id WHERE e.%1 = :id").arg(Enums::ExtrasType::getColumn(type));
    query.prepare(sql);
    query.bindValue(":id", id);

    if (!query.exec()) {
        return query.lastError();
    }

    while (query.next()) {
        ExtraListItem item(query.record());
        QSqlError err = m_playbacks->loadPlaybacks(db, item.fileId, item.playback);
        if (err.type() != QSqlError::NoError) {
            return err;
        }
        result.append(item);
    }

    return QSqlError();
}
