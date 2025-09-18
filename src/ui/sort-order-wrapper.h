#pragma once

#include <QObject>
#include "../model/sort-order.h"

namespace Rd {
    namespace Ui {
        class SortOrderWrapper {
            Q_GADGET
        public:
            enum WrappedSortOrder {
                TitleAsc = SortOrder::TitleAsc,
                TitleDesc = SortOrder::TitleDesc,
                ReleaseAsc = SortOrder::ReleaseAsc,
                ReleaseDesc = SortOrder::ReleaseDesc
            };
            Q_ENUM(WrappedSortOrder)

            static SortOrder toSortOrder(WrappedSortOrder order);
            static WrappedSortOrder toWrappedSortOrder(SortOrder order);
        };
    }
}
