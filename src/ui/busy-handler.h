#pragma once

#include <QObject>

namespace Rd {
    namespace Ui {
        class BusyHandler : public QObject {
            Q_OBJECT
            Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyUpdated)
        public:
            static BusyHandler* instance();

            bool busy();
            void setBusy(bool busy);
            Q_SIGNAL void busyUpdated();
        private:
            bool m_busy = false;

            explicit BusyHandler(QObject* parent = nullptr);
        };
    }
}
