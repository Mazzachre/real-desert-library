#include "mode-handler.h"

Rd::Ui::ModeHandler::ModeHandler(QObject* parent)
: QObject(parent) {
    m_mode = About;
}

Rd::Ui::ModeHandler::Mode Rd::Ui::ModeHandler::mode() const {
    return m_mode;
}

void Rd::Ui::ModeHandler::setMode(Mode mode) {
    m_mode = mode;
    Q_EMIT modeUpdated();
}

int Rd::Ui::ModeHandler::modeTabIndex() {
    switch(m_mode) {
        case Shows:
        case Show:
            return 0;
        case Movies:
        case Movie:
            return 1;
        default:
            return -1;
    }
}

void Rd::Ui::ModeHandler::setModeByIndex(int index) {
    switch(index) {
        case 0:
            setMode(Shows);
            break;
        case 1:
            setMode(Movies);
            break;
        default:
            setMode(About);
    }
}

Rd::Ui::ModeHandler * Rd::Ui::ModeHandler::instance() {
    static ModeHandler instance;
    return &instance;
}
