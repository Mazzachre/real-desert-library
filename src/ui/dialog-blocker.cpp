#include "dialog-blocker.h"

Rd::Ui::DialogBlocker::DialogBlocker(QObject* parent)
: QObject(parent) {
}

bool Rd::Ui::DialogBlocker::block() {
    return m_count > 0;
}

void Rd::Ui::DialogBlocker::reset() {
    m_count = 0;
    Q_EMIT blockUpdated();
}

void Rd::Ui::DialogBlocker::opened() {
    ++m_count;
    Q_EMIT blockUpdated();
}

void Rd::Ui::DialogBlocker::closed() {
    if (m_count > 0) {
        --m_count;
        Q_EMIT blockUpdated();
    }
}
