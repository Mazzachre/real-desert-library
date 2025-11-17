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

    explicit ShowFilter();

    QString query() const;
    QVariantMap values() const;
};
