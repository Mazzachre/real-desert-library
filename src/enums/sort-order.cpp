#include "sort-order.h"

Rd::Enums::SortOrder::SortOrder(QObject* parent) : QObject(parent) {}

Rd::Enums::SortOrder* Rd::Enums::SortOrder::instance()  {
    static SortOrder instance;
    return &instance;
}
