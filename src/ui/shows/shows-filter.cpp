#include "shows-filter.h"

Rd::Ui::Shows::ShowsFilter::ShowsFilter(QObject* parent)
: QObject(parent)
, m_db{new Rd::Database::Shows} {
}

Rd::Ui::Shows::ShowsFilter::~ShowsFilter() noexcept {
    m_db->deleteLater();
}

void Rd::Ui::Shows::ShowsFilter::init() {
    m_filter.newerThan = 0;
    m_filter.olderThan = 0;
    QMap<quint16, QString> genres;
    QSqlError err = m_db->getGenres(genres);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading show genre list", err.text());
        return;
    }
    m_genres.clear();
    auto gKeys = genres.keys();
    for (auto key : gKeys) {
        QVariantMap item;
        item["id"] = key;
        item["name"] = genres.value(key);
        item["selected"] = false;
        m_genres.append(item);
    }

    std::sort(m_genres.begin(), m_genres.end(), [](const QVariant& a, QVariant& b) {
        return a.toMap().value("name").toString().toLower() < b.toMap().value("name").toString().toLower();
    });

    QMap<quint16, QString> tags;
    err = m_db->getTags(tags);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading show tags list", err.text());
        return;
    }

    m_tags.clear();
    auto tKeys = tags.keys();
    for (auto key : tKeys) {
        QVariantMap item;
        item["id"] = key;
        item["name"] = tags.value(key);
        item["selected"] = false;
        m_tags.append(item);
    }

    std::sort(m_tags.begin(), m_tags.end(), [](const QVariant& a, QVariant& b) {
        return a.toMap().value("name").toString().toLower() < b.toMap().value("name").toString().toLower();
    });

    Q_EMIT filterUpdated();
}

void Rd::Ui::Shows::ShowsFilter::clear() {
    m_filter.reset();
    m_genres.clear();
    m_tags.clear();
}

quint16 Rd::Ui::Shows::ShowsFilter::newerThan() {
    return m_filter.newerThan;
}

void Rd::Ui::Shows::ShowsFilter::setNewerThan(quint16 value) {
    m_filter.newerThan = value;
    Q_EMIT filterUpdated();
}

quint16 Rd::Ui::Shows::ShowsFilter::olderThan() {
    return m_filter.olderThan;
}

void Rd::Ui::Shows::ShowsFilter::setOlderThan(quint16 value) {
    m_filter.olderThan = value;
    Q_EMIT filterUpdated();
}

bool Rd::Ui::Shows::ShowsFilter::watching() {
    return m_filter.watching;
}

void Rd::Ui::Shows::ShowsFilter::setWatching(bool watching) {
    m_filter.watching = watching;
    Q_EMIT filterUpdated();
}

QVariantList Rd::Ui::Shows::ShowsFilter::genres() {
    return m_genres;
}

void Rd::Ui::Shows::ShowsFilter::toggleGenre(quint16 genre) {
    for (auto& item : m_genres) {
        auto map = item.toMap();
        if (map["id"].toUInt() == genre) {
            if (map["selected"].toBool()) {
                m_filter.genres.removeAll(genre);
                map["selected"] = false;
            } else {
                m_filter.genres.append(genre);
                map["selected"] = true;
            }
            item = map;
            break;
        }
    }
    Q_EMIT filterUpdated();
}

QVariantList Rd::Ui::Shows::ShowsFilter::tags() {
    return m_tags;
}

void Rd::Ui::Shows::ShowsFilter::toggleTag(quint16 tag) {
    for (auto& item : m_tags) {
        auto map = item.toMap();
        if (map["id"].toUInt() == tag) {
            if (map["selected"].toBool()) {
                m_filter.tags.removeAll(tag);
                map["selected"] = false;
            } else {
                m_filter.tags.append(tag);
                map["selected"] = true;
            }
            item = map;
            break;
        }
    }
    Q_EMIT filterUpdated();
}

ShowFilter Rd::Ui::Shows::ShowsFilter::filter() {
    return m_filter;
}

void Rd::Ui::Shows::ShowsFilter::setPerson(quint32 id) {
    m_filter.person = id;
    Q_EMIT filterUpdated();
}

void Rd::Ui::Shows::ShowsFilter::reset() {
    clear();
    init();
    Q_EMIT filterUpdated();
}
