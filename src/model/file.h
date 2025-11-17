#pragma once

#include <QDebug>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QList>
#include <QDate>
#include <QSqlRecord>

struct File {
    quint32 id;
    QString path;
    QDate created;
    QVariant selectedSubtitle;
    QVariant selectedVideo;
    QVariant selectedAudio;
    QVariantMap meta;
    quint16 runtime;
    QVariantList subtitles;
    QVariantList video;
    QVariantList audio;

    File();
    explicit File(QString path);

    bool isCreated();
    bool isEmpty();
};

QDebug operator<<(QDebug dbg, const File& data);

Q_DECLARE_METATYPE(File);
