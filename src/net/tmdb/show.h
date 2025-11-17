#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QMap>
#include <QVector>

namespace Rd {
    namespace Net {
        namespace Tmdb {
            class Show : public QObject {
                Q_OBJECT
            public:
                explicit Show(QObject* parent = nullptr);
                ~Show() noexcept;

                Q_SLOT void search(const QString& req, quint16 page = 1);
                Q_SIGNAL void searchResult(const QJsonObject& result);

                Q_SLOT void get(quint32 id);
                Q_SIGNAL void getResult(const QJsonObject& result);

                Q_SLOT void episodes(quint32 id, const QVector<quint8>& seasons);
                Q_SIGNAL void episodesResult(const QJsonObject& result);

                Q_SIGNAL void error(const QString& text);
            private:
                enum RequestType {
                    Search,
                    Get,
                    Episodes
                };

                QMap<QNetworkReply*, RequestType> m_requests;
                QNetworkAccessManager* m_net;
                Q_SLOT void handleResult(QNetworkReply* reply);
            };
        }
    }
}
