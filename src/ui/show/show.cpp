#include "show.h"
#include <QDebug>

Rd::Ui::Show::Show::Show(QObject* parent)
: QObject(parent)
, m_shows_db{new Rd::Database::Shows}
, m_episodes_db{new Rd::Database::Episodes}
, m_episodes{new EpisodeList}
, m_matcher{new EpisodeMatch}
, m_details{new EpisodeDetails} {
    connect(m_matcher, &EpisodeMatch::episodesMatched, this, &Show::handleEpisodesMatched);
    connect(m_episodes, &EpisodeList::favoriteSet, this, &Show::handleFavoriteSet);
    connect(m_episodes, &EpisodeList::detailsOpened, m_details, &EpisodeDetails::setEpisode);
    connect(m_details, &EpisodeDetails::favoriteSet, this, &Show::handleFavoriteSet);
}

Rd::Ui::Show::Show::~Show() noexcept {
    m_details->deleteLater();
    m_matcher->deleteLater();
    m_episodes->deleteLater();
    m_episodes_db->deleteLater();
    m_shows_db->deleteLater();
}

void Rd::Ui::Show::Show::loadShow(quint32 id) {
    QSqlError err = m_shows_db->getShow(id, m_show);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading show", err.text());
        return;
    }

    loadEpisodes();
}

void Rd::Ui::Show::Show::clear() {
    m_episodes->clear();
    m_matcher->clear();
}

QString Rd::Ui::Show::Show::name() {
    return m_show.name;
}

QString Rd::Ui::Show::Show::originalName() {
    return m_show.originalName;
}

QString Rd::Ui::Show::Show::overview() {
    return m_show.overview;
}

QAbstractListModel* Rd::Ui::Show::Show::episodes() {
    return m_episodes;
}

QAbstractListModel * Rd::Ui::Show::Show::matcher() {
    return m_matcher;
}

QObject * Rd::Ui::Show::Show::details() {
    return m_details;
}

void Rd::Ui::Show::Show::loadEpisodes() {
    QList<EpisodeListItem> episodes;
    QSqlError err = m_episodes_db->loadEpisodes(m_show.id, episodes);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading show", err.text());
        return;
    }

    m_episodes->setEpisodes(episodes);
    m_matcher->setEpisodes(episodes);
    Q_EMIT showLoaded();
}

void Rd::Ui::Show::Show::handleEpisodesMatched(QMap<quint32, quint32> matches) {
    QSqlError err = m_episodes_db->linkEpisodes(matches);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading show", err.text());
        return;
    }

    loadEpisodes();
}

void Rd::Ui::Show::Show::handleFavoriteSet(quint32 id, bool favorite) {
    QSqlError err = m_episodes_db->setFavorite(id, favorite);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error setting favorite episode", err.text());
        return;
    }
    m_episodes->updateFavorite(id, favorite);
}
