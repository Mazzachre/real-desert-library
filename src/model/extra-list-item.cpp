#include "extra-list-item.h"

ExtraListItem::ExtraListItem(const QSqlRecord& record) {
    id = record.value("id").toUInt();
    name = record.value("name").toString();
    fileId = record.value("file_id").toUInt();
    path = record.value("path").toString();
    runtime = record.value("runtime").toUInt();
}

QDebug operator<<(QDebug dbg, const ExtraListItem& data) {
    dbg.nospace().noquote() << "Extra:(\"" << data.name << "\" \"" << data.path << "\")";
    return dbg;
}
