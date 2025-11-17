#include "playback.h"
#include "lib/format.h"

Playback::Playback(quint32 fileId, const QDateTime& time, quint32 played) {
    this->fileId = fileId;
    this->time = time;
    this->played = played;
}

Playback::Playback(const QSqlRecord& record) {
    fileId = record.value("file_id").toUInt();
    time = record.value("time").toDateTime();
    played = record.value("played").toUInt();
}

QVariantMap Playback::toMap(quint32 runtime) const {
    return QVariantMap({
        {"time", time.toString()},
        {"played", QString("%1 of %2")
            .arg(Rd::Library::formatRuntime(played/60))
            .arg(Rd::Library::formatRuntime(runtime))}
    });
}

QDebug operator<<(QDebug dbg, const Playback& data) {
    dbg.nospace().noquote() << "Playback:(" << data.fileId << " " << data.time << " " << data.played << ")";
    return dbg;
}
