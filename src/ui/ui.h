#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "./shows/shows.h"
#include "./show/show.h"
#include "./movies/movies.h"
#include "./movie/movie.h"

namespace Rd {
    namespace Ui {
        class Ui : public QObject {
            Q_OBJECT
        public:
            explicit Ui(QObject* parent = nullptr);
            ~Ui() noexcept;
            void show(const QRect& dimensions);

        private:
            QQmlApplicationEngine* m_engine;
            QQuickWindow* m_window = nullptr;
            QObject* m_loader = nullptr;
            Rd::Ui::Shows::Shows* m_shows;
            Rd::Ui::Show::Show* m_show;
            Rd::Ui::Movies::Movies* m_movies;
            Rd::Ui::Movie::Movie* m_movie;

            Q_SLOT void handleModeChange();
            Q_SLOT void handleError(const QString& headline, const QString& errorText);
            Q_SLOT void handleWarning(const QString& text);
        };
    }
}
