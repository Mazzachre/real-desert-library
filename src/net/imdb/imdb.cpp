#include "imdb.h"

Rd::Net::Imdb::Imdb::Imdb(QObject* parent)
: QObject(parent)
, m_net{new QNetworkAccessManager}
, m_reply{nullptr}
, m_downloadFile{nullptr}
, m_decompressedFile{nullptr}
, m_gzipProcess{new QProcess} {
    connect(m_gzipProcess, &QProcess::readyReadStandardOutput, this, &Imdb::onGzipOutput);
    connect(m_gzipProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &Imdb::onGzipFinished);
}

Rd::Net::Imdb::Imdb::~Imdb() noexcept {
    m_net->deleteLater();
    m_gzipProcess->deleteLater();
}

void Rd::Net::Imdb::Imdb::fetchBasics() {
    if (m_reply != nullptr || m_downloadFile != nullptr || m_decompressedFile != nullptr) {
        Q_EMIT error("Already downloading IMDb file");
        return;
    }

    m_downloadFile = new QTemporaryFile(this);
    if (!m_downloadFile->open()) {
        Q_EMIT error(QString("Failed to open %1 for writing.").arg(m_downloadFile->fileName()));
        cleanUp();
        return;
    }

    m_decompressedFile = new QTemporaryFile(this);
    if (!m_decompressedFile->open()) {
        Q_EMIT error(QString("Failed to open %1 for writing.").arg(m_decompressedFile->fileName()));
        cleanUp();
        return;
    }

    QUrl url("https://datasets.imdbws.com/title.basics.tsv.gz");
    QNetworkRequest request(url);
    m_reply = m_net->get(request);

    connect(m_reply, &QNetworkReply::readyRead, this, &Imdb::onReadyRead);
    connect(m_reply, &QNetworkReply::finished, this, &Imdb::onDownloadFinished);
    connect(m_reply, &QNetworkReply::downloadProgress, [this](quint64 recieved, quint64 total) {
        Q_EMIT progress(recieved, total);
    });
}

void Rd::Net::Imdb::Imdb::onReadyRead() {
    if (m_reply && m_downloadFile->isOpen()) {
        m_downloadFile->write(m_reply->readAll());
    }
}

void Rd::Net::Imdb::Imdb::onDownloadFinished() {
    if (m_reply->error() != QNetworkReply::NoError) {
        Q_EMIT error(m_reply->errorString());
        cleanUp();
        return;
    }

    m_downloadFile->flush();
    m_downloadFile->close();
    m_reply->deleteLater();
    m_reply = nullptr;

    m_gzipProcess->setProgram("gzip");
    m_gzipProcess->setArguments({ "-dc", m_downloadFile->fileName() });
    m_gzipProcess->start();
}

void Rd::Net::Imdb::Imdb::onGzipOutput() {
    QByteArray data = m_gzipProcess->readAllStandardOutput();
    m_decompressedFile->write(data);
}

void Rd::Net::Imdb::Imdb::onGzipFinished(int exitCode, QProcess::ExitStatus status) {
    m_decompressedFile->close();
    m_downloadFile->deleteLater();
    m_downloadFile = nullptr;

    if (exitCode == 0 && status == QProcess::NormalExit) {
        Q_EMIT basicFetched(m_decompressedFile);
        m_decompressedFile = nullptr;
    } else {
        Q_EMIT error("Decompression failed");
        cleanUp();
    }
}

void Rd::Net::Imdb::Imdb::cleanUp() {
    if (m_reply) {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
    if (m_downloadFile) {
        m_downloadFile->deleteLater();
        m_downloadFile = nullptr;
    }
    if (m_decompressedFile) {
        m_decompressedFile->deleteLater();
        m_decompressedFile = nullptr;
    }
}
