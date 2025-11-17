#pragma once

#include <QObject>

namespace Rd {
    namespace Enums {
        class ExtrasType : public QObject {
            Q_OBJECT
        public:
            enum Type {
                Movie,
                Show,
                Episode,
            };
            Q_ENUM(Type);

            static QString getColumn(Type type);
            static ExtrasType* instance();
        private:
            explicit ExtrasType(QObject* parent = nullptr);
        };
    }
}
