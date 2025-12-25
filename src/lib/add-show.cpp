#include "add-show.h"
#include "ui/busy-handler.h"
#include <QDebug>
#include <QVariant>
#include <QSqlDatabase>
#include <QJsonArray>

Rd::Library::AddShow::AddShow(QObject* parent)
: QObject(parent)
, m_show_db{new Database::Shows}
, m_episode_db{new Database::Episodes}
, m_show_net{new Net::Tmdb::Show} {
    connect(m_show_net, &Net::Tmdb::Show::getResult, this, &AddShow::handleShowResult);
    connect(m_show_net, &Net::Tmdb::Show::episodesResult, this, &AddShow::handleEpisodeResult);
    connect(m_show_net, &Net::Tmdb::Show::error, this, [this](const QString& text) {
        Q_EMIT error(u"Error adding show: "_qs, text);
    });
}

Rd::Library::AddShow::~AddShow() noexcept {
    m_episode_db->deleteLater();
    m_show_db->deleteLater();
    m_show_net->deleteLater();
}

void Rd::Library::AddShow::addShow(quint32 id) {
    bool exists = false;
    QSqlError err = m_show_db->exists(id, exists);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error adding show", err.text());
        return;
    }
    if (exists) {
        Q_EMIT showAdded(id);
        return;
    }
    m_show_net->get(id);
    Ui::BusyHandler::instance()->setBusy(true);
}

void Rd::Library::AddShow::handleShowResult(const QJsonObject& result) {
    Show show(result);
    m_add = show;
    quint32 id = result[u"id"].toInt();

    for (const QJsonValue& val : result[u"seasons"].toArray()) {
        QJsonObject season = val.toObject();
        quint8 season_number = season[u"season_number"].toInt();
        if (season_number > 0) {
            m_seasons.append(season_number);
        }
    }

    if (m_seasons.count() > 19) {
        m_show_net->episodes(id, m_seasons.mid(0, 19));
    } else {
        m_show_net->episodes(id, m_seasons);
    }
}

void Rd::Library::AddShow::handleEpisodeResult(const QJsonObject& result) {
    quint32 id = result[u"id"].toInt();
    QList<quint8> fetched = m_seasons.mid(0, 19);

    for(const quint8 seasonNumber : fetched) {
        QString seasonKey = QString("season/%1").arg(seasonNumber);
        QJsonObject season = result[seasonKey].toObject();
        QJsonArray episodes = season[u"episodes"].toArray();
        for (const QJsonValue& val : episodes) {
            Episode episode(val.toObject());
            m_episodes.append(episode);
        }
    }

    m_seasons = m_seasons.mid(19);

    if (m_seasons.count() > 19) {
        m_show_net->episodes(id, m_seasons.mid(0, 19));
    } else if (m_seasons.count() > 0) {
        m_show_net->episodes(id, m_seasons);
    } else {
        QSqlDatabase db = QSqlDatabase::database();

        if (!db.transaction()) {
            Q_EMIT error("Error adding show", db.lastError().text());
            return;
        }

        QSqlError showError = m_show_db->saveShow(m_add);
        if (showError.type() != QSqlError::NoError) {
            Q_EMIT error("Error adding show", showError.text());
            db.rollback();
            return;
        }

        QSqlError episodesError = m_episode_db->saveEpisodes(m_episodes);
        if (episodesError.type() != QSqlError::NoError) {
            Q_EMIT error("Error adding show", episodesError.text());
            db.rollback();
            return;
        }

        if (!db.commit()) {
            Q_EMIT error("Error adding show", db.lastError().text());
            db.rollback();
            return;
        }

        m_episodes.clear();
        m_seasons.clear();
        Ui::BusyHandler::instance()->setBusy(false);
        Q_EMIT showAdded(id);
    }
}
