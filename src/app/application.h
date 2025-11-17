#pragma once

#include <QObject>
#include <QApplication>
#include "init/database.h"
#include "init/imdb.h"
#include "ui/ui.h"

namespace Rd {
    namespace Application {
        class Application : public QObject {
           Q_OBJECT
        public:
            explicit Application(QApplication* app, QObject *parent = nullptr);
            ~Application() noexcept;

            void start();

        private:
            QRect m_dimensions;
            Init::Database* m_db;
            Init::Imdb* m_imdb;
            Ui::Ui* m_ui;

            Q_SLOT void close();
            Q_SLOT void handleError(const QString& text);
            Q_SLOT void onInitDone();

            void showSplash();
            Q_SLOT void splashMessage(const QString& text);
            void hideSplash();
        };
    }
}
