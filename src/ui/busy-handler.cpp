#include "busy-handler.h"

#include <QDebug>

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
    static BusyHandler instance;
    return &instance;
}
