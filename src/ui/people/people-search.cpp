#include "people-search.h"

void Rd::Ui::People::PeopleSearch::search(const QString& name) {
    beginResetModel();
    m_people.clear();
    QSqlError err = m_db->findPeople(name, m_people);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error searching for people", err.text());
    }
    endResetModel();
}

void Rd::Ui::People::PeopleSearch::clear() {
    beginResetModel();
    m_people.clear();
    endResetModel();
}

QHash<int, QByteArray> Rd::Ui::People::PeopleSearch::roleNames() const {
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {OriginalNameRole, "originalName"},
        {ProfilePathRole, "profilePath"},
    };
}

int Rd::Ui::People::PeopleSearch::rowCount(const QModelIndex& parent) const {
    return m_people.count();
}

QVariant Rd::Ui::People::PeopleSearch::data(const QModelIndex& index, int role) const {
    if (index.isValid()) {
        const Person& item = m_people[index.row()];
        switch(role) {
            case IdRole:
                return item.id;
            case NameRole:
                return item.name;
            case OriginalNameRole:
                return item.originalName;
            case ProfilePathRole:
                return item.profilePath;
        }
    }

    return QVariant();
}

Rd::Ui::People::PeopleSearch::PeopleSearch(QObject* parent)
: QAbstractListModel(parent)
, m_db{new Rd::Database::CastCrew} {}

Rd::Ui::People::PeopleSearch::~PeopleSearch() noexcept {
    m_db->deleteLater();
}
