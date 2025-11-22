#include "person-search.h"
#include <QList>
#include "model/person.h"

Rd::Ui::People::PersonSearch::PersonSearch(QObject* parent)
: QObject(parent)
, m_db {new Database::CastCrew} {
}

Rd::Ui::People::PersonSearch::~PersonSearch() noexcept {
    m_db->deleteLater();
}

void Rd::Ui::People::PersonSearch::clear() {
    m_people.clear();
    Q_EMIT peopleUpdated();
    m_selected.clear();
    Q_EMIT selectedUpdated();
}

QVariantList Rd::Ui::People::PersonSearch::people() {
    return m_people;
}

void Rd::Ui::People::PersonSearch::search(const QString& text) {
    QList<Person> people;
    QSqlError err = m_db->findPeople(text, people);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error searching for people", err.text());
    }
    m_people.clear();
    for (auto& person : people) {
        QVariantMap map = QVariantMap({
            {"id", person.id},
            {"name", person.name},
            {"originalName", person.originalName},
            {"profile", person.profilePath}
        });
        m_people.append(map);
    }
    Q_EMIT peopleUpdated();
}

QString Rd::Ui::People::PersonSearch::selected() {
    return m_selected;
}

void Rd::Ui::People::PersonSearch::setSelected(const QString& selected) {
    m_selected = selected;
    Q_EMIT selectedUpdated();
}
