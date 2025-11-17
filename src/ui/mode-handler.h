#pragma once

#include <QObject>

namespace Rd {
    namespace Ui {
        class ModeHandler : public QObject {
            Q_OBJECT
            Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeUpdated)
        public:
            enum Mode {
                About,
                Movies,
                Shows,
                Movie,
                Show,
                Extras
            };
            Q_ENUM(Mode)

            Mode mode() const;
            Q_INVOKABLE void setMode(Mode mode);
            Q_INVOKABLE int modeTabIndex();
            Q_INVOKABLE void setModeByIndex(int index);
            Q_SIGNAL void modeUpdated();

            static ModeHandler* instance();
        private:
            Mode m_mode = Shows;

            explicit ModeHandler(QObject* parent = nullptr);
        };
    }
}
