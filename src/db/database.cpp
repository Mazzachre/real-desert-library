#include "database.h"
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QSqlError>

Rd::Database::Database::Database(QObject* parent)
: QObject(parent) {}

Rd::Database::Database::~Database() noexcept {}

QSqlError Rd::Database::Database::open() {
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir;
    if (!dir.exists(dataPath)) {
        if (!dir.mkpath(dataPath)) {
            return QSqlError(
                "Application initialization error",
                QString("Could not create data directory: %1").arg(dataPath),
                QSqlError::UnknownError
            );
        }
    }

    QString dbPath = dataPath + QDir::separator() + "real-desert-library.sqlite";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);


    if (!db.open()) {
        return db.lastError();
    }

    return QSqlError();
}

QSqlError Rd::Database::Database::migrate() {
    Rd::Database::Migration::Migrate migration;
    return migration.run();
}

void Rd::Database::Database::close() {
    if (QSqlDatabase::database().isOpen()) {
        QSqlDatabase::database().close();
    }
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}
