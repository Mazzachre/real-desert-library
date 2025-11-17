#include "tags.h"

#include <QSqlDatabase>
#include <QSqlQuery>

Rd::Database::Tags::Tags(QObject* parent)
: QObject(parent) {
}

Rd::Database::Tags::~Tags() noexcept {
}

QSqlError Rd::Database::Tags::getTags(QVariantList& tags) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT id, name FROM tags ORDER BY name ASC");
    if (!query.exec()) return query.lastError();
    while(query.next()) {
        QVariantMap item;
        item.insert("id", query.value("id"));
        item.insert("name", query.value("name"));
        tags.append(item);
    }
    return QSqlError();

}

QSqlError Rd::Database::Tags::addTag(const QString& tag, QVariantMap& added) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO tags(name) VALUES(:name)");
    query.bindValue(":name", QVariant(tag));
    if (!query.exec()) return query.lastError();
    QVariant idVariant = query.lastInsertId();
    if (!idVariant.isValid()) {
        QSqlQuery selectQuery(db);
        selectQuery.prepare("SELECT id, name FROM tags WHERE name = :name");
        selectQuery.bindValue(":name", QVariant(tag));
        if (!selectQuery.exec() || !selectQuery.next())
            return QSqlError("", QString("Unable to retrieve existing ID for tag '%1'").arg(tag), QSqlError::UnknownError);
        added.insert("id", selectQuery.value("id"));
    } else {
        added.insert("id", idVariant);
        added.insert("name", tag);
    }
    return QSqlError();
}
