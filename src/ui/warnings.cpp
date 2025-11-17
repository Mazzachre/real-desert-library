#include "warnings.h"
#include <QDateTime>

Rd::Ui::Warnings::Warnings(QObject* parent)
: QAbstractListModel(parent) {
}

Rd::Ui::Warnings::~Warnings() noexcept {
}

void Rd::Ui::Warnings::handleWarning(const QString& text) {
    beginInsertRows(QModelIndex(), m_warnings.count(), m_warnings.count());
    QVariantMap item;
    item.insert("text", text);
    item.insert("time", QDateTime::currentDateTime());
    m_warnings.append(item);
    endInsertRows();
}

void Rd::Ui::Warnings::close(int at) {
    if (at >= 0 && at < m_warnings.count()) {
        beginRemoveRows(QModelIndex(), at, at);
        m_warnings.removeAt(at);
        endRemoveRows();
    }
}

QHash<int, QByteArray> Rd::Ui::Warnings::roleNames() const {
    return {
        {TextRole, "text"},
        {TimeRole, "time"},
    };
}

int Rd::Ui::Warnings::rowCount(const QModelIndex& parent) const {
    return m_warnings.count();
}

QVariant Rd::Ui::Warnings::data(const QModelIndex& index, int role) const {
    if (index.isValid()) {
        if (role == TextRole) return m_warnings[index.row()].toMap().value("text");
        if (role == TimeRole) return m_warnings[index.row()].toMap().value("time").toString();
    }
    return QVariant();
}
