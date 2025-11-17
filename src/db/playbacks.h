#pragma once

#include <QObject>
#include <QSqlError>
#include <QList>
#include <QSqlDatabase>
#include "model/playback.h"

namespace Rd {
    namespace Database {
        class Playbacks : public QObject {
            Q_OBJECT
        public:
            explicit Playbacks(QObject* parent = nullptr);
            ~Playbacks() noexcept;

            QSqlError savePlayback(const Playback& playback);
            QSqlError loadPlaybacks(const QSqlDatabase& db, quint32 fileId, QList<Playback>& result);
        };
    }
}
