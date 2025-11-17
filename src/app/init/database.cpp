#include "database.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include "migration/migrate.h"

Rd::Application::Init::Database::Database(QObject* parent)
: QObject(parent) {
}

Rd::Application::Init::Database::~Database() noexcept {
}

void Rd::Application::Init::Database::start() {
    Q_EMIT progress("Initializing database");
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir;
    if (!dir.exists(dataPath)) {
        if (!dir.mkpath(dataPath)) {
            Q_EMIT error(QString("Could not create data directory: %1").arg(dataPath));
            return;
        }
    }

    QString dbPath = dataPath + QDir::separator() + "real-desert-library.sqlite";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        Q_EMIT error(db.lastError().text());
        return;
    }

    Rd::Database::Migration::Migrate migration;
    QSqlError err = migration.run();
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error(err.text());
        return;
    }

    Q_EMIT done();
}
