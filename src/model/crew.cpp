#include "crew.h"

Crew::Crew(const QJsonObject& obj) {
    person = Person(obj);
    job = obj[u"department"].toString() + u":"_qs + obj[u"job"].toString();
}

Crew::Crew(const QSqlRecord& rec) {
    job = rec.value("job").toString();
    person = Person(rec);
}

QDebug operator<<(QDebug dbg, const Crew& data) {
    dbg.nospace().noquote() << "Crew:(" << data.job << " " << data.person << ")";
    return dbg;
}
