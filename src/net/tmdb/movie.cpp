#include "movie.h"
#include "../../../keys.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>

Rd::Net::Tmdb::Movie::Movie(QObject* parent)
: QObject(parent)
, m_net{new QNetworkAccessManager} {
    connect(m_net, &QNetworkAccessManager::finished, this, &Movie::handleResult);
}

Rd::Net::Tmdb::Movie::~Movie() noexcept {
    m_net->deleteLater();
}

void Rd::Net::Tmdb::Movie::search(const QString& req, quint16 page) {
    QSettings settings;

    QUrlQuery query;

    query.addQueryItem(u"query"_qs, req);
    query.addQueryItem(u"include_adult"_qs, u"false"_qs);
    query.addQueryItem(u"language"_qs, settings.value(u"language"_qs, "en-US").toString());
    query.addQueryItem(u"page"_qs, QString::number(page));
    query.addQueryItem(u"sort_by"_qs, "popularity");

    QUrl url(u"https://api.themoviedb.org/3/search/movie"_qs);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("accept"), QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("Authorization"), QByteArrayLiteral(TMDB_TOKEN));

    QNetworkReply* reply = m_net->get(request);
    m_requests.insert(reply, Search);
}

void Rd::Net::Tmdb::Movie::get(quint32 id) {
    QSettings settings;

    QUrlQuery query;

    query.addQueryItem(u"language"_qs, settings.value(u"language"_qs, "en-US").toString());
    query.addQueryItem(u"append_to_response"_qs, u"credits,keywords,external_ids"_qs);

    QUrl url(u"https://api.themoviedb.org/3/movie/"_qs + QString::number(id));
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("accept"), QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("Authorization"), QByteArrayLiteral(TMDB_TOKEN));

    QNetworkReply* reply = m_net->get(request);
    m_requests.insert(reply, Get);
}

void Rd::Net::Tmdb::Movie::handleResult(QNetworkReply* reply) {
    if (!m_requests.contains(reply)) {
        Q_EMIT error("Unknown TMDB response");
        reply->deleteLater();
        return;
    }

    RequestType type = m_requests.take(reply);

    QByteArray result = reply->readAll();
    qDebug() << "Result" << result << Qt::endl;

    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT error(reply->errorString() + ": " +QString(result));
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
    }

    reply->deleteLater();
}

