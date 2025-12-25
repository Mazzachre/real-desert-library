#pragma once

#include <QString>
#include <QList>
#include <QVariant>

struct MovieFilter {
    explicit MovieFilter();
    void reset();

    QString query() const;
    QVariantMap values() const;
};
