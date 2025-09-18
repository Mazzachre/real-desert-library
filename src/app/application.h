#pragma once

#include <QObject>
#include <QRect>
#include "../db/database.h"
#include "../ui/ui.h"

namespace Rd {
    namespace Application {
        class Application : public QObject {
           Q_OBJECT
        public:
            explicit Application(const QRect& dimensions, QObject *parent = nullptr);
            ~Application() noexcept;

            bool init();
            void run();
            void close();
        private:
            QRect m_dimensions;
            Rd::Database::Database* m_db;
            Rd::Ui::Ui* m_ui;

            void showSplash();
            void hideSplash();
        };
    }
}
