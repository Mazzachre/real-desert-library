#include "cast-list.h"

Rd::Ui::Show::CastList::CastList(QObject* parent)
: QAbstractListModel(parent) {
}

Rd::Ui::Show::CastList::~CastList() noexcept {
}

void Rd::Ui::Show::CastList::setEpisodes(const QList<EpisodeListItem>& episodes) {
    m_people.clear();
    for (auto& episode : episodes) {
        for (auto& cast : episode.cast) {
            if (!m_count.contains(cast.person.id)) {
                m_people.append(cast.person);
                m_count.insert(cast.person.id, 1);
            } else {
                m_count[cast.person.id] = m_count[cast.person.id]+1;
            }
        }
    }
    std::sort(m_people.begin(), m_people.end());
    filter();
}

void Rd::Ui::Show::CastList::clear() {
    m_people.clear();
    filter();
}

QHash<int, QByteArray> Rd::Ui::Show::CastList::roleNames() const {
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {CountRole, "count"},
    };
}

int Rd::Ui::Show::CastList::rowCount(const QModelIndex& parent) const {
    return m_filtered.count();
}

QVariant Rd::Ui::Show::CastList::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        const Person& person = m_filtered[index.row()];
        switch (role) {
            case IdRole: {
                return person.id;
            }
            case NameRole: {
                return person.name;
            }
            case CountRole: {
                return m_count[person.id];
            }
        }
    }
    return QVariant();
}

void Rd::Ui::Show::CastList::filter() {
    beginResetModel();
    m_filtered.clear();
    if (m_filter.isEmpty()) {
        m_filtered = m_people;
    } else {
        const QString needle = m_filter.toLower();
        for (const Person &p : m_people) {
            if (p.name.toLower().contains(needle))
                m_filtered.append(p);
        }
    }
    endResetModel();
}
