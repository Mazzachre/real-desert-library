#include "busy-handler.h"

#include <QDebug>

// Rd::Ui::BusyHandler* Rd::Ui::BusyHandler::m_self = nullptr;

Rd::Ui::BusyHandler::BusyHandler(QObject* parent)
: QObject(parent) {
}

bool Rd::Ui::BusyHandler::busy() {
    return m_busy;
}

void Rd::Ui::BusyHandler::setBusy(bool busy) {
    m_busy = busy;
    qDebug() << "Setting busy" << busy;
    Q_EMIT busyUpdated();
}

Rd::Ui::BusyHandler* Rd::Ui::BusyHandler::instance() {
    // if (m_self == nullptr) {
    //     m_self = new BusyHandler();
    // }
    // return m_self;
    static BusyHandler instance;
    return &instance;
}
