#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTemporaryFile>
#include <QProcess>
#include <QDebug>

namespace Rd {
    namespace Net {
        namespace Imdb {
            class Imdb : public QObject {
                Q_OBJECT
            public:
                explicit Imdb(QObject* parent = nullptr);
                ~Imdb() noexcept;

                Q_SLOT void fetchBasics();
                Q_SIGNAL void progress(quint64 recieved, quint64 total);
                Q_SIGNAL void basicFetched(QTemporaryFile* file);
                Q_SIGNAL void error(const QString& text);
            private:
                QNetworkAccessManager* m_net;
                QNetworkReply* m_reply;
                QTemporaryFile* m_downloadFile;
                QTemporaryFile* m_decompressedFile;
                QProcess* m_gzipProcess;

                Q_SLOT void onReadyRead();
                Q_SLOT void onDownloadFinished();
                Q_SLOT void onGzipOutput();
                Q_SLOT void onGzipFinished(int exitCode, QProcess::ExitStatus status);

                void cleanUp();
            };
        }
    }
}
