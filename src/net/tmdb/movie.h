#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QMap>

namespace Rd {
    namespace Net {
        namespace Tmdb {
            class Movie : public QObject {
                Q_OBJECT
            public:
                explicit Movie(QObject* parent = nullptr);
                ~Movie() noexcept;

                Q_SLOT void search(const QString& req, quint16 page = 1);
                Q_SIGNAL void searchResult(const QJsonObject& result);

                Q_SLOT void get(quint32 id);
                Q_SIGNAL void getResult(const QJsonObject& result);

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                enum RequestType {
                    Search,
                    Get
                };

                QMap<QNetworkReply*, RequestType> m_requests;
                QNetworkAccessManager* m_net;

                Q_SLOT void handleResult(QNetworkReply* reply);
            };
        }
    }
}
