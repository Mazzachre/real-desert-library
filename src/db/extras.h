#pragma once

#include <QObject>
#include <QSqlError>
#include <QList>
#include "../enums/extras-type.h"
#include "../model/extra-list-item.h"
#include "../model/extra.h"
#include "playbacks.h"

namespace Rd {
    namespace Database {
        class Extras : public QObject {
            Q_OBJECT
        public:
            Extras(QObject* parent = nullptr);
            ~Extras() noexcept;

            QSqlError saveExtras(const QList<Extra>& items);
            QSqlError updateName(quint32 id, const QString& name);
            QSqlError loadExtras(Rd::Enums::ExtrasType::Type type, quint32 id, QList<ExtraListItem>& result);
        private:
            Playbacks* m_playbacks;
        };
    }
}
