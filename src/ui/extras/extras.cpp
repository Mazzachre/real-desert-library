#include "extras.h"
#include "model/extra-list-item.h"
#include <QDebug>

Rd::Ui::Extras::Extras::Extras(QObject* parent)
: QAbstractListModel(parent)
, m_db{new Database::Extras}
, m_add(new AddExtras) {
    connect(m_add, &AddExtras::extrasAdded, this, &Extras::handleExtrasAdded);
}

Rd::Ui::Extras::Extras::~Extras() noexcept {
    m_add->deleteLater();
    m_db->deleteLater();
}

QAbstractListModel * Rd::Ui::Extras::Extras::add() {
    return m_add;
}

void Rd::Ui::Extras::Extras::load(Rd::Enums::ExtrasType::Type type, quint32 id) {
    m_type = type;
    m_id = id;
    reload();
    Q_EMIT extrasLoaded();
}

void Rd::Ui::Extras::Extras::back() {
    Q_EMIT goBack(m_type, m_id);
}

void Rd::Ui::Extras::Extras::clear() {
    beginResetModel();
    m_extras.clear();
    m_selected = 0;
    endResetModel();
    Q_EMIT selectedUpdated();
}

quint32 Rd::Ui::Extras::Extras::selected() {
    return m_selected;
}

void Rd::Ui::Extras::Extras::setSelected(quint32 selected) {
    m_selected = selected;
    Q_EMIT selectedUpdated();
}

void Rd::Ui::Extras::Extras::updateName(quint32 id, const QString& name) {
    for (int i = 0; i < m_extras.count(); ++i) {
        auto& extra = m_extras[i];
        if (extra.id == id) {
            QSqlError err = m_db->updateName(id, name);
            if (err.type() != QSqlError::NoError) {
                Q_EMIT warn("Error updating name for extra: " + err.text());
                return;
            }
            extra.name = name;
            QModelIndex modelIndex = index(i);
            Q_EMIT dataChanged(modelIndex, modelIndex, {NameRole});
        }
    }
}

QHash<int, QByteArray> Rd::Ui::Extras::Extras::roleNames() const {
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {RuntimeRole, "runtime"},
        {PlayedRole, "played"},
        {PlayedFullyRole, "playedFully"},
        {PlaybackListRole, "playbackList"},
    };
}

int Rd::Ui::Extras::Extras::rowCount(const QModelIndex& parent) const {
    return m_extras.count();
}

QVariant Rd::Ui::Extras::Extras::data(const QModelIndex& index, int role) const {
    if (index.isValid()) {
        auto& item = m_extras[index.row()];
        switch(role) {
            case IdRole:
                return item.id;
            case NameRole:
                return item.name;
            case RuntimeRole: {
                QString runtime = QString("%1:%2")
                    .arg(item.runtime / 60)
                    .arg(item.runtime % 60, 2, 10, QChar('0'));
                return runtime;
            }
            case PlayedRole:
                return !item.playback.isEmpty();
            case PlayedFullyRole: {
                if (item.playback.isEmpty()) return false;
                quint32 lastPlayed = item.playback[0].played;
                quint32 runtime = item.runtime*60;
                quint32 diff = runtime - lastPlayed;
                return diff > runtime/10;
            }
            case PlaybackListRole: {
                QVariantList result;
                for (auto& playback : item.playback.first(5)) {
                    result << playback.toMap(item.runtime);
                }
                return result;
            }
        }
    }

    return QVariant();
}

void Rd::Ui::Extras::Extras::reload() {
    beginResetModel();
    m_extras.clear();

    QSqlError err = m_db->loadExtras(m_type, m_id, m_extras);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading extras", err.text());
        return;
    }
    endResetModel();
}

void Rd::Ui::Extras::Extras::handleExtrasAdded(const QMap<quint32, QString>& entries) {
    QList<Extra> extras;

    for (auto it = entries.constBegin(); it != entries.constEnd(); ++it) {
        Extra extra;
        extra.type = m_type;
        extra.id = m_id;
        extra.fileId = it.key();
        extra.name = it.value();
        extras.append(extra);
    }

    QSqlError err = m_db->saveExtras(extras);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT warn("Error adding extras: " + err.text());
        return;
    }
    reload();
}
