#include "shows-tags-list.h"

Rd::Ui::Shows::ShowTagsList::ShowTagsList(QObject* parent)
: QAbstractListModel(parent) {
}

Rd::Ui::Shows::ShowTagsList::~ShowTagsList() noexcept {
}

void Rd::Ui::Shows::ShowTagsList::setTags(const QMap<quint16, QString>& tags) {
    Q_EMIT beginResetModel();
    m_data.clear();
    auto keys = tags.keys();
    for (auto key : keys) {
        QVariantMap item;
        item["id"] = key;
        item["name"] = tags.value(key);
        item["selected"] = false;
        m_data.append(item);
    }

    std::sort(m_data.begin(), m_data.end(), [](const QVariantMap& a, QVariantMap& b) {
        return a["name"].toString().toLower() < b["name"].toString().toLower();
    });
    Q_EMIT endResetModel();
    Q_EMIT selectedUpdated();
}

void Rd::Ui::Shows::ShowTagsList::clear() {
    m_data.clear();
}

QList<quint16> Rd::Ui::Shows::ShowTagsList::selected() {
    QList<quint16> retval;
    for (const auto& it : m_data) {
        if (it["selected"].toBool()) {
            retval.append(it["id"].toInt());
        }
    }
    return retval;
}

void Rd::Ui::Shows::ShowTagsList::toggle(quint16 id) {
    for (int i = 0; i < m_data.count(); ++i) {
        if(m_data[i].value("id").toInt() == id) {
            m_data[i].insert("selected", !m_data[i].value("selected").toBool());
            Q_EMIT dataChanged(index(i), index(i), {SelectedRole});
            break;
        }
    }
    Q_EMIT selectedUpdated();
}

int Rd::Ui::Shows::ShowTagsList::rowCount(const QModelIndex& parent) const {
    return m_data.count();
}

QHash<int, QByteArray> Rd::Ui::Shows::ShowTagsList::roleNames() const {
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {SelectedRole, "selected"}
    };
}

QVariant Rd::Ui::Shows::ShowTagsList::data(const QModelIndex& index, int role) const {
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

