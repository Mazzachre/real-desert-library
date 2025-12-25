#pragma once

#include <QObject>
#include "model/movie-filter.h"

namespace Rd {
    namespace Ui {
        namespace Movies {
            class MoviesFilter : public QObject {
                Q_OBJECT
            public:
                explicit MoviesFilter(QObject* parent = nullptr);
                ~MoviesFilter() noexcept;

                void init();
                void clear();

                MovieFilter filter();
                Q_SIGNAL void filterUpdated();

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                MovieFilter m_filter;
            };
        }
    }
}
