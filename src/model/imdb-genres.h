#pragma once

#include <QDebug>
#include <QString>
#include <QList>

struct ImdbGenres {
    QString id;
    QStringList genres;
};

QDebug operator<<(QDebug dbg, const ImdbGenres& data);

Q_DECLARE_METATYPE(ImdbGenres);
