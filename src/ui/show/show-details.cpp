#include "show-details.h"

Rd::Ui::Show::ShowDetails::ShowDetails(QObject* parent)
: QObject(parent) {
}

Rd::Ui::Show::ShowDetails::~ShowDetails() noexcept {
}

void Rd::Ui::Show::ShowDetails::setShow(const struct Show& show) {
    m_show = show;
    Q_EMIT showUpdated();
}

void Rd::Ui::Show::ShowDetails::clear() {
    m_show = ::Show();
}

quint32 Rd::Ui::Show::ShowDetails::id() {
    return m_show.id;
}

QString Rd::Ui::Show::ShowDetails::name() {
    return m_show.name;
}

QString Rd::Ui::Show::ShowDetails::originalName() {
    return m_show.originalName;
}

bool Rd::Ui::Show::ShowDetails::hasExtras() {
    return m_show.hasExtras;
}

QVariantList Rd::Ui::Show::ShowDetails::info() {
    QVariantList data;
    data.append(QVariantMap({
        {"key", "Original Name"},
        {"value", m_show.originalName}
    }));
    data.append(QVariantMap({
        {"key", "Tagline"},
        {"value", m_show.tagline}
    }));
    data.append(QVariantMap({
        {"key", "Overview"},
        {"value", m_show.overview}
    }));
    data.append(QVariantMap({
        {"key", "Origin"},
        {"value", m_show.origin.join(", ")}
    }));
    data.append(QVariantMap({
        {"key", "Languages"},
        {"value", m_show.languages.join(", ")}
    }));
    return data;
}

QVariantList Rd::Ui::Show::ShowDetails::genres() {
    QVariantList data;
    for (auto& it : m_show.genres) {
        QVariantMap item;
        item["id"] = it.id;
        item["name"] = it.name;
        data.append(item);
    }
    return data;
}

QVariantList Rd::Ui::Show::ShowDetails::tags() {
    return m_show.tags;
}

QVariantList Rd::Ui::Show::ShowDetails::crew() {
    QVariantList list;

    for (auto& it : m_show.crew) {
        QVariantMap item;
        item.insert("value", it.job);
        item.insert("name", it.person.name);
        list.append(QVariant(item));
    }

    return list;
}

QVariantList Rd::Ui::Show::ShowDetails::cast() {
    QVariantList list;

    for (auto& it : m_show.cast) {
        QVariantMap item;
        item.insert("value", it.role);
        item.insert("name", it.person.name);
        list.append(QVariant(item));
    }

    return list;
}
