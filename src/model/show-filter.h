#pragma once

#include <QString>
#include <QList>
#include <QVariant>

struct ShowFilter {
    quint16 newerThan;
    quint16 olderThan;
    QList<quint16> genres;
    QList<quint16> tags;
    bool watching;
    quint32 person;

    explicit ShowFilter();
    void reset();

    QString query() const;
    QVariantMap values() const;
};
