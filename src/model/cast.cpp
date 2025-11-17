#include "cast.h"

Cast::Cast(const QJsonObject& obj) {
    person = Person(obj);
    role = obj[u"character"].toString();
}

Cast::Cast(const QSqlRecord& rec) {
    role = rec.value("role").toString();
    person = Person(rec);
}

bool Cast::operator<(const Cast& other) const {
    return person < other.person;
}

QDebug operator<<(QDebug dbg, const Cast& data) {
    dbg.nospace().noquote() << "Cast:(" << data.role << " " << data.person << ")";
    return dbg;
}
