#include "person.h"

#include <QCollator>

Person::Person() {
}

Person::Person(const QJsonObject& data) {
    id = data[u"id"].toInt();
    name = data[u"name"].toString();
    originalName = data[u"original_name"].toString();
    profilePath = data[u"profile_path"].toString();
}

Person::Person(const QSqlRecord& record) {
    id = record.value("id").toInt();
    name = record.value("name").toString();
    originalName = record.value("original_name").toString();
    profilePath = record.value("profile_path").toString();
}

static QStringList effectiveParts(const QString &name) {
    static const QSet<QString> ignoredSuffixes = {
        "jr", "sr", "i", "ii", "iii", "iv", "v", "vi"
    };

    QStringList parts = name.toLower().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return parts;

    if (ignoredSuffixes.contains(parts.last()))
        parts.removeLast();

    return parts;
}

bool Person::operator<(const Person& other) const {
    static QCollator collator;
    collator.setNumericMode(true);
    collator.setCaseSensitivity(Qt::CaseInsensitive);

    QStringList partsA = effectiveParts(name);
    QStringList partsB = effectiveParts(other.name);

    int iA = partsA.size() - 1;
    int iB = partsB.size() - 1;

    while (iA >= 0 && iB >= 0) {
        int cmp = collator.compare(partsA[iA], partsB[iB]);
        if (cmp != 0)
            return cmp < 0;
        --iA;
        --iB;
    }

    if (iA < iB) return true;
    if (iA > iB) return false;

    return name.toLower() < other.name.toLower();
}

QDebug operator<<(QDebug dbg, const Person& data) {
    dbg.nospace().noquote() << "Person:(" << data.id << " " <<  data.name << ")";
    return dbg;
}
