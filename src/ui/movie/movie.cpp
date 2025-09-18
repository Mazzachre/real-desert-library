#include "movie.h"
#include <QDebug>

Rd::Ui::Movie::Movie::Movie(QObject* parent)
: QObject(parent) {
}

Rd::Ui::Movie::Movie::~Movie() noexcept {
}

void Rd::Ui::Movie::Movie::loadMovie(quint32 id) {
    qDebug() << "Load movie" << id;
}

void Rd::Ui::Movie::Movie::clear() {
}
