#include "migrate.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "initial.h"

Rd::Database::Migration::Migrate::Migrate(QObject* parent)
: QObject(parent) {
    m_migrations.emplace_back(std::make_unique<Initial>());
}

Rd::Database::Migration::Migrate::~Migrate() noexcept {
}

QSqlError Rd::Database::Migration::Migrate::run() {
    int version = 0;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlError initError = init(db);
    if (initError.type() != QSqlError::NoError) {
        return initError;
    }
    QSqlError versionError = lastVersion(db, version);
    if (versionError.type() != QSqlError::NoError) {
        return versionError;
    }

    QSqlError result;

    for (const auto& migration : m_migrations) {
        if (migration->version() > version) {
            if (!db.transaction()) {
                result = db.lastError();
                break;
            }
            QSqlError applyError = migration->apply(db);
            if (applyError.type() != QSqlError::NoError) {
                result = applyError;
                db.rollback();
                break;
            }
            QSqlError updateError = update(db, migration->version());
            if (updateError.type() != QSqlError::NoError) {
                result = updateError;
                db.rollback();
                break;
            }
            if (!db.commit()) {
                result = db.lastError();
                db.rollback();
                break;
            }
        }
    }
    return result;
}

QSqlError Rd::Database::Migration::Migrate::init(const QSqlDatabase& db) {
    QSqlQuery query(db);
    query.prepare("CREATE TABLE IF NOT EXISTS versions (version INT, applied TIMESTAMP default CURRENT_TIMESTAMP)");
    query.exec();
    return query.lastError();
}

QSqlError Rd::Database::Migration::Migrate::lastVersion(const QSqlDatabase& db, int& version) {
    QSqlQuery query(db);
    query.prepare("SELECT MAX(version) FROM versions");
    if (!query.exec()) {
        return query.lastError();
    }

    if (query.next()) {
        version = query.value(0).toInt();
    }
    return QSqlError();
}

QSqlError Rd::Database::Migration::Migrate::update(const QSqlDatabase& db, int version) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO versions (version) VALUES (:version)");
    query.bindValue(":version", version);
    query.exec();
    return query.lastError();
}
