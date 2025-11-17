#include "genres.h"
#include <QSqlDatabase>
#include <QSqlQuery>

Rd::Database::Genres::Genres(QObject* parent)
: QObject(parent) {
}

Rd::Database::Genres::~Genres() noexcept {
}

QSqlError Rd::Database::Genres::getGenres(QList<Genre>& genres) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT id, name, source FROM genres ORDER BY name ASC");
    if (!query.exec()) return query.lastError();
    while(query.next()) {
        genres.append(Genre(query.record()));
    }
    return QSqlError();
}

QSqlError Rd::Database::Genres::addGenre(const QString& genre, Genre& added) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO genres(name, source) VALUES(:name, 'USER')");
    query.bindValue(":name", QVariant(genre));
    if (!query.exec()) return query.lastError();
    QVariant idVariant = query.lastInsertId();
    if (!idVariant.isValid()) {
        QSqlQuery selectQuery(db);
        selectQuery.prepare("SELECT id, name, source FROM genres WHERE name = :name");
        selectQuery.bindValue(":name", QVariant(genre));
        if (!selectQuery.exec() || !selectQuery.next())
            return QSqlError("", QString("Unable to retrieve existing ID for genre '%1'").arg(genre), QSqlError::UnknownError);
        added = Genre(selectQuery.record());
    } else {
        added.id = idVariant.toUInt();
        added.name = genre;
        added.source = "USER";
    }
    return QSqlError();
}
