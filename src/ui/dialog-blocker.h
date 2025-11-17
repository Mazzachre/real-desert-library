#pragma once

#include <QObject>

namespace Rd {
    namespace Ui {
        class DialogBlocker : public QObject {
            Q_OBJECT
            Q_PROPERTY(bool block READ block NOTIFY blockUpdated)
        public:
            explicit DialogBlocker(QObject* parent = nullptr);

            bool block();
            void reset();
            Q_SIGNAL void blockUpdated();

            Q_INVOKABLE void opened();
            Q_INVOKABLE void closed();
        private:
            int m_count;
        };
    }
}
