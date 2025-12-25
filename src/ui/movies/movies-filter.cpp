#include "movies-filter.h"

Rd::Ui::Movies::MoviesFilter::MoviesFilter(QObject* parent)
: QObject(parent) {
}

Rd::Ui::Movies::MoviesFilter::~MoviesFilter() noexcept {
}

void Rd::Ui::Movies::MoviesFilter::init() {
    Q_EMIT filterUpdated();
}

void Rd::Ui::Movies::MoviesFilter::clear() {
    m_filter.reset();
}

MovieFilter Rd::Ui::Movies::MoviesFilter::filter() {
    return m_filter;
}
