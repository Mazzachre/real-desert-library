#include "genre.h"

#include <QVariant>

Genre::Genre(){
}

Genre::Genre(const QSqlRecord& record) {
    id = record.value("id").toUInt();
    name = record.value("name").toString();
    source = record.value("source").toString();
}

bool Genre::operator==(const Genre& other) const {
    return id == other.id;
}

bool Genre::operator<(const Genre& other) const {
    return name < other.name;
}

QDebug operator<<(QDebug dbg, const Genre& data) {
    dbg.nospace().noquote() << "Genre:(" << data.id << " " << data.name << " " << data.source << ")";
    return dbg;
}
