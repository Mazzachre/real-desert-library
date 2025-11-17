#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "shows/shows.h"
#include "show/show.h"
#include "movies/movies.h"
#include "movie/movie.h"
#include "extras/extras.h"
#include "warnings.h"
#include "dialog-blocker.h"

namespace Rd {
    namespace Ui {
        class Ui : public QObject {
            Q_OBJECT
        public:
            explicit Ui(QObject* parent = nullptr);
            ~Ui() noexcept;
            void show(const QRect& dimensions);
            void setVisibility(bool visible);

            Q_SIGNAL void error(const QString& text);
        private:
            QQmlApplicationEngine* m_engine;
            QQuickWindow* m_window = nullptr;
            QObject* m_loader = nullptr;
            Shows::Shows* m_shows;
            Show::Show* m_show;
            Movies::Movies* m_movies;
            Movie::Movie* m_movie;
            Extras::Extras* m_extras;
            Warnings* m_warnings;
            DialogBlocker* m_blocker;

            Q_SLOT void handleModeChange();
            Q_SLOT void handleError(const QString& headline, const QString& errorText);
        };
    }
}
