#include "shows-filter.h"

Rd::Ui::Shows::ShowsFilter::ShowsFilter(QObject* parent)
: QObject(parent)
, m_db{new Rd::Database::Shows}
, m_genres{new ShowGenreList}
, m_tags{new ShowTagsList} {
    connect(m_genres, &ShowGenreList::selectedUpdated, this, [this](){
        m_filter.genres = m_genres->selected();
        Q_EMIT filterUpdated();
    });
    connect(m_tags, &ShowTagsList::selectedUpdated, this, [this](){
        m_filter.tags = m_tags->selected();
        Q_EMIT filterUpdated();
    });
}

Rd::Ui::Shows::ShowsFilter::~ShowsFilter() noexcept {
    m_tags->deleteLater();
    m_genres->deleteLater();
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
    m_genres->setGenres(genres);

    QMap<quint16, QString> tags;
    err = m_db->getTags(tags);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading show tags list", err.text());
        return;
    }
    m_tags->setTags(tags);
}

void Rd::Ui::Shows::ShowsFilter::clear() {
    m_filter.newerThan = 0;
    m_filter.olderThan = 0;
    m_filter.genres.clear();
    m_filter.tags.clear();
    m_genres->clear();
    m_tags->clear();
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

QAbstractListModel* Rd::Ui::Shows::ShowsFilter::genres() {
    return m_genres;
}

QAbstractListModel * Rd::Ui::Shows::ShowsFilter::tags() {
    return m_tags;
}

ShowFilter Rd::Ui::Shows::ShowsFilter::filter() {
    return m_filter;
}
