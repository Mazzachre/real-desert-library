#pragma once

#include <QDate>
#include <QSqlRecord>

struct Playback {
    quint32 fileId;
    QDateTime time;
    quint32 played;

    Playback(quint32 fileId, const QDateTime& time, quint32 played);
    explicit Playback(const QSqlRecord& record);
    QVariantMap toMap(quint32 runtime) const;
};

Q_DECLARE_METATYPE(Playback)

QDebug operator<<(QDebug dbg, const Playback& data);
