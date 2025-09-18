#include "shows-genre-list.h"

Rd::Ui::Shows::ShowGenreList::ShowGenreList(QObject* parent)
: QAbstractListModel(parent) {
}

Rd::Ui::Shows::ShowGenreList::~ShowGenreList() noexcept {
}

void Rd::Ui::Shows::ShowGenreList::setGenres(const QMap<quint16, QString>& genres) {
    Q_EMIT beginResetModel();
    m_data.clear();
    auto keys = genres.keys();
    for (auto key : keys) {
        QVariantMap item;
        item["id"] = key;
        item["name"] = genres.value(key);
        item["selected"] = false;
        m_data.append(item);
    }

    std::sort(m_data.begin(), m_data.end(), [](const QVariantMap& a, QVariantMap& b) {
        return a["name"].toString().toLower() < b["name"].toString().toLower();
    });
    Q_EMIT endResetModel();
    Q_EMIT selectedUpdated();
}

void Rd::Ui::Shows::ShowGenreList::clear() {
    m_data.clear();
}

QList<quint16> Rd::Ui::Shows::ShowGenreList::selected() {
    QList<quint16> retval;
    for (const auto& it : m_data) {
        if (it["selected"].toBool()) {
            retval.append(it["id"].toInt());
        }
    }
    return retval;
}

void Rd::Ui::Shows::ShowGenreList::toggle(quint16 id) {
    for (int i = 0; i < m_data.count(); ++i) {
        if(m_data[i].value("id").toInt() == id) {
            m_data[i].insert("selected", !m_data[i].value("selected").toBool());
            Q_EMIT dataChanged(index(i), index(i), {SelectedRole});
            break;
        }
    }
    Q_EMIT selectedUpdated();
}

int Rd::Ui::Shows::ShowGenreList::rowCount(const QModelIndex& parent) const {
    return m_data.count();
}

QHash<int, QByteArray> Rd::Ui::Shows::ShowGenreList::roleNames() const {
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {SelectedRole, "selected"}
    };
}

QVariant Rd::Ui::Shows::ShowGenreList::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        QVariantMap data = m_data.at(index.row());
        switch(role) {
            case IdRole:
                return data["id"];
            case NameRole:
                return data["name"];
            case SelectedRole:
                return data["selected"];
        }
    }

    return QVariant();
}
