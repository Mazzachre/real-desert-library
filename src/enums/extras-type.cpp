#include "extras-type.h"

Rd::Enums::ExtrasType::ExtrasType(QObject* parent) : QObject(parent) {}

QString Rd::Enums::ExtrasType::getColumn(Type type) {
    switch (type) {
        case Episode:
            return "episode_id";
        case Movie:
            return "movie_id";
        case Show:
            return "show_id";
    }

    return QString();
}

Rd::Enums::ExtrasType* Rd::Enums::ExtrasType::instance()  {
    static ExtrasType instance;
    return &instance;
}

