#include "show.h"
#include <QDebug>

Rd::Ui::Show::Show::Show(QObject* parent)
: QObject(parent)
, m_localPath{new Library::LocalPath}
, m_shows_db{new Database::Shows}
, m_episodes_db{new Database::Episodes}
, m_files_db{new Database::Files}
, m_genre_db{new Database::Genres}
, m_tags_db{new Database::Tags}
, m_play_db{new Database::Playbacks}
, m_show_details{new ShowDetails}
, m_episodes{new EpisodeList}
, m_matcher{new EpisodeMatch}
, m_details{new EpisodeDetails}
, m_cast{new CastList} {
    connect(m_matcher, &EpisodeMatch::episodesMatched, this, &Show::handleEpisodesMatched);
    connect(m_episodes, &EpisodeList::detailsOpened, m_details, &EpisodeDetails::setEpisode);
    connect(this, &Show::favoriteSet, m_details, &EpisodeDetails::updateFavorite);
    connect(this, &Show::favoriteSet, m_episodes, &EpisodeList::updateFavorite);
    connect(this, &Show::subtitleSet, m_episodes, &EpisodeList::updateSubtitle);
    connect(this, &Show::playedSet, m_episodes, &EpisodeList::updatePlayed);
    connect(this, &Show::episodesLoaded, m_matcher, &EpisodeMatch::setEpisodes);
    connect(this, &Show::episodesLoaded, m_episodes, &EpisodeList::setEpisodes);
    connect(this, &Show::episodesLoaded, m_cast, &CastList::setEpisodes);
}

Rd::Ui::Show::Show::~Show() noexcept {
    m_cast->deleteLater();
    m_details->deleteLater();
    m_matcher->deleteLater();
    m_episodes->deleteLater();
    m_show_details->deleteLater();
    m_play_db->deleteLater();
    m_tags_db->deleteLater();
    m_genre_db->deleteLater();
    m_files_db->deleteLater();
    m_episodes_db->deleteLater();
    m_shows_db->deleteLater();
    m_localPath->deleteLater();
}

void Rd::Ui::Show::Show::load(quint32 id) {
    QSqlError err = m_shows_db->getShow(id, m_show);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading show", err.text());
        return;
    }

    m_show_details->setShow(m_show);
    loadGenres();
    loadTags();
    loadEpisodes();
    Q_EMIT showLoaded();
}

void Rd::Ui::Show::Show::loadByEpisodeId(quint32 id) {
    quint32 showId;
    QSqlError err = m_shows_db->findByEpisode(id, showId);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading show", err.text());
        return;
    }

    load(showId);
}

void Rd::Ui::Show::Show::clear() {
    m_episodes->clear();
    m_matcher->clear();
    m_details->clear();
    m_show_details->clear();
    m_cast->clear();
    m_show = ::Show();
    m_genres.clear();
    m_tags.clear();
}

QAbstractListModel* Rd::Ui::Show::Show::episodes() {
    return m_episodes;
}

QAbstractListModel * Rd::Ui::Show::Show::matcher() {
    return m_matcher;
}

QAbstractListModel * Rd::Ui::Show::Show::cast() {
    return m_cast;
}


QObject * Rd::Ui::Show::Show::episode() {
    return m_details;
}

QObject * Rd::Ui::Show::Show::show() {
    return m_show_details;
}

QVariantList Rd::Ui::Show::Show::genres() {
    QVariantList genres;
    for (auto& genre : m_genres) {
        if (!m_show.genres.contains(genre)) {
            QVariantMap item;
            item["id"] = genre.id;
            item["name"] = genre.name;
            genres.append(item);
        }
    }
    return genres;
}

QVariantList Rd::Ui::Show::Show::tags() {
    QVariantList tags;
    QSet<quint32> ids;
    for (auto& item : m_show.tags) {
        ids.insert(item.toMap().value("id").toUInt());
    }

    for (auto& item : m_tags) {
        if (!ids.contains(item.toMap().value("id").toUInt())) {
            tags.append(item);
        }
    }
    return tags;
}

void Rd::Ui::Show::Show::setFavorite(quint32 id, bool favorite) {
    QSqlError err = m_episodes_db->setFavorite(id, favorite);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT warn("Error setting favorite episode: " + err.text());
        return;
    }

    Q_EMIT favoriteSet(id, favorite);
}

void Rd::Ui::Show::Show::setSubtitle(quint32 fileId, const QUrl& subtitle) {
    QString path = subtitle.isEmpty() ? QString() : m_localPath->reverseLocalPath(subtitle.toLocalFile()).toString();
    QSqlError err = m_files_db->setSubtitle(fileId, path);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT warn("Error setting subtitle: " + err.text());
        return;
    }

    Q_EMIT subtitleSet(fileId, path);
}

void Rd::Ui::Show::Show::setPlayed(quint32 fileId, quint32 played) {
    Playback playback(fileId, QDateTime::currentDateTime(), played);
    QSqlError err = m_play_db->savePlayback(playback);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT warn("Error setting played: " + err.text());
        return;
    }

    Q_EMIT playedSet(playback);
}

void Rd::Ui::Show::Show::removeGenre(quint32 id) {
    QSqlError err = m_shows_db->unlinkGenre(m_show.imdb, id);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT warn("Error removing genre: " + err.text());
        return;
    }

    for (int i = 0; i < m_show.genres.size(); ++i) {
        if (m_show.genres[i].id == id) {
            m_show.genres.removeAt(i);
            m_show_details->setShow(m_show);
            Q_EMIT genresUpdated();
            break;
        }
    }
}

void Rd::Ui::Show::Show::addGenre(const QVariantMap& genre) {
    if (genre.contains("id")) {
        linkGenre(genre.value("id").toUInt());
    } else {
        Genre g;
        QSqlError err = m_genre_db->addGenre(genre.value("name").toString(), g);
        if (err.type() != QSqlError::NoError) {
            Q_EMIT warn("Error creating genre: " + err.text());
            return;
        }
        auto it = std::lower_bound(m_genres.begin(), m_genres.end(), g, [](const Genre& a, const Genre& b) {
            return a < b;
        });
        m_genres.insert(it, g);
        linkGenre(g.id);
    }
}

void Rd::Ui::Show::Show::removeTag(quint32 id) {
    QSqlError err = m_shows_db->unlinkTag(m_show.id, id);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT warn("Error removing tag: " + err.text());
        return;
    }

    for (int i = 0; i < m_show.tags.size(); ++i) {
        auto map = m_show.tags[i].toMap();
        if (map.value("id").toUInt() == id) {
            m_show.tags.removeAt(i);
            m_show_details->setShow(m_show);
            Q_EMIT genresUpdated();
            break;
        }
    }
}

void Rd::Ui::Show::Show::addTag(const QVariantMap& tag) {
    if (tag.contains("id")) {
        linkTag(tag.value("id").toUInt());
    } else {
        QVariantMap t;
        QSqlError err = m_tags_db->addTag(tag.value("name").toString(), t);
        if (err.type() != QSqlError::NoError) {
            Q_EMIT warn("Error creating tag: " + err.text());
            return;
        }
        auto it = std::lower_bound(m_tags.begin(), m_tags.end(), t, [](const QVariant& a, const QVariant& b) {
            return a.toMap().value("name").toString() < b.toMap().value("name").toString();
        });
        m_tags.insert(it, t);
        linkTag(t.value("id").toUInt());
    }
}

void Rd::Ui::Show::Show::loadEpisodes() {
    QList<EpisodeListItem> episodes;
    QSqlError err = m_episodes_db->loadEpisodes(m_show.id, episodes);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading show", err.text());
        return;
    }

    Q_EMIT episodesLoaded(episodes);
}

void Rd::Ui::Show::Show::handleEpisodesMatched(QMap<quint32, quint32> matches) {
    QSqlError err = m_episodes_db->linkEpisodes(matches);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT warn("Error linking episodes to show: " + err.text());
        return;
    }

    loadEpisodes();
}

void Rd::Ui::Show::Show::loadGenres() {
    QSqlError err = m_genre_db->getGenres(m_genres);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading genres", err.text());
        return;
    }
    Q_EMIT genresUpdated();
}

void Rd::Ui::Show::Show::loadTags() {
    QSqlError err = m_tags_db->getTags(m_tags);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading tags", err.text());
        return;
    }
    Q_EMIT tagsUpdated();
}

void Rd::Ui::Show::Show::linkGenre(quint32 id) {
    QSqlError err = m_shows_db->linkGenre(m_show.imdb, id);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT warn("Error linking genre: " + err.text());
        return;
    }
    for (auto& genre : m_genres) {
        if (genre.id == id) {
            auto it = std::lower_bound(m_show.genres.begin(), m_show.genres.end(), genre, [](const Genre& a, const Genre& b) {
                return a < b;
            });
            m_show.genres.insert(it, genre);
            m_show_details->setShow(m_show);
            Q_EMIT genresUpdated();
            break;
        }
    }
}

void Rd::Ui::Show::Show::linkTag(quint32 id) {
    QSqlError err = m_shows_db->linkTag(m_show.id, id);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT warn("Error linking tag: " + err.text());
        return;
    }
    for (auto& tag : m_tags) {
        if (tag.toMap().value("id").toUInt() == id) {
            auto it = std::lower_bound(m_show.tags.begin(), m_show.tags.end(), tag, [](const QVariant& a, const QVariant& b) {
                return a.toMap().value("name").toString() < b.toMap().value("name").toString();
            });
            m_show.tags.insert(it, tag);
            m_show_details->setShow(m_show);
            Q_EMIT tagsUpdated();
            break;
        }
    }
}
