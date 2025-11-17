#include "playbacks.h"
#include <QSqlQuery>

Rd::Database::Playbacks::Playbacks(QObject* parent)
: QObject(parent) {
}

Rd::Database::Playbacks::~Playbacks() noexcept {
}

QSqlError Rd::Database::Playbacks::savePlayback(const Playback& playback) {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("INSERT INTO playbacks(file_id, time, played) VALUES(:file_id, :time, :played)");
    query.bindValue(":file_id", playback.fileId);
    query.bindValue(":time", QVariant(playback.time.toString(Qt::ISODate)));
    query.bindValue(":played", playback.played);

    query.exec();
    return query.lastError();
}

QSqlError Rd::Database::Playbacks::loadPlaybacks(const QSqlDatabase& db, quint32 fileId, QList<Playback>& result) {
    QSqlQuery query(db);
    query.prepare("SELECT file_id, time, played FROM playbacks WHERE file_id = :file_id ORDER BY time DESC");
    query.bindValue(":file_id", fileId);

    if (!query.exec()) return query.lastError();

    while (query.next()) {
        Playback playback = Playback(query.record());
        result << playback;
    }

    return QSqlError();
}
