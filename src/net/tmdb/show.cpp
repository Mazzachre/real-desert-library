#include "show.h"
#include "../../../keys.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>

Rd::Net::Tmdb::Show::Show(QObject* parent)
: QObject(parent)
, m_net{new QNetworkAccessManager} {
    connect(m_net, &QNetworkAccessManager::finished, this, &Show::handleResult);
}

Rd::Net::Tmdb::Show::~Show() noexcept {
    m_net->deleteLater();
}

void Rd::Net::Tmdb::Show::search(const QString& req, quint16 page) {
    QSettings settings;

    QUrlQuery query;

    query.addQueryItem(u"query"_qs, req);
    query.addQueryItem(u"include_adult"_qs, u"false"_qs);
    query.addQueryItem(u"language"_qs, settings.value(u"language"_qs, "en-US").toString());
    query.addQueryItem(u"page"_qs, QString::number(page));
    query.addQueryItem(u"sort_by"_qs, "popularity");

    QUrl url(u"https://api.themoviedb.org/3/search/tv"_qs);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("accept"), QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("Authorization"), QByteArrayLiteral(TMDB_TOKEN));

    QNetworkReply* reply = m_net->get(request);
    m_requests.insert(reply, Search);
}

void Rd::Net::Tmdb::Show::get(quint32 id) {
    QSettings settings;

    QUrlQuery query;

    query.addQueryItem(u"language"_qs, settings.value(u"language"_qs, "en-US").toString());
    query.addQueryItem(u"append_to_response"_qs, u"credits,keywords,external_ids"_qs);


    QUrl url(u"https://api.themoviedb.org/3/tv/"_qs + QString::number(id));
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("accept"), QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("Authorization"), QByteArrayLiteral(TMDB_TOKEN));

    QNetworkReply* reply = m_net->get(request);
    m_requests.insert(reply, Get);
}

void Rd::Net::Tmdb::Show::episodes(quint32 id, const QVector<quint8>& seasons) {
    QSettings settings;
    QUrlQuery query;

    QStringList seasonList;
    for (quint8 season : seasons) {
        seasonList << QString("season/%1").arg(season);
    }

    query.addQueryItem(u"language"_qs, settings.value(u"language"_qs, "en-US").toString());
    query.addQueryItem(u"append_to_response"_qs, seasonList.join(','));

    QUrl url(u"https://api.themoviedb.org/3/tv/"_qs + QString::number(id));
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("accept"), QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("Authorization"), QByteArrayLiteral(TMDB_TOKEN));

    QNetworkReply* reply = m_net->get(request);
    m_requests.insert(reply, Episodes);
}

void Rd::Net::Tmdb::Show::handleResult(QNetworkReply* reply) {
    if (!m_requests.contains(reply)) {
        Q_EMIT error("Unknown TMDB response");
        reply->deleteLater();
        return;
    }

    RequestType type = m_requests.take(reply);

    QByteArray result = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT error(reply->errorString() + ": " + QString(result));
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(result, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        Q_EMIT error(jsonError.errorString());
        return;
    }

    switch(type) {
        case Search:
            Q_EMIT searchResult(doc.object());
            break;
        case Get:
            Q_EMIT getResult(doc.object());
            break;
        case Episodes:
            Q_EMIT episodesResult(doc.object());
            break;
    }

    reply->deleteLater();
}
