#pragma once

#include <QObject>

namespace Rd {
    namespace Enums {
        class SortOrder : public QObject {
            Q_OBJECT
        public:
            enum Order {
                TitleAsc,
                TitleDesc,
                ReleaseAsc,
                ReleaseDesc
            };
            Q_ENUM(Order);

            static SortOrder* instance();
        private:
            explicit SortOrder(QObject* parent = nullptr);
        };
    }
}
