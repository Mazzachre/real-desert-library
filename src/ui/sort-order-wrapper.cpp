#include "sort-order-wrapper.h"

SortOrder Rd::Ui::SortOrderWrapper::toSortOrder(WrappedSortOrder order) {
    return static_cast<SortOrder>(order);
}
