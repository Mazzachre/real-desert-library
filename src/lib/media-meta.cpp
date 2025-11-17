#include "media-meta.h"
#include <QMimeData>
#include <QMimeDatabase>
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavutil/dict.h>
}

QString resolution(int width, int height) {
    int max_dim = std::max(width, height);

    if (max_dim >= 3840) return "UHD";
    if (max_dim >= 2560) return "QHD";
    if (max_dim >= 1920) return "FHD";
    if (max_dim >= 1280) return "HD";
    return "SD";
}

Rd::Library::MediaMeta::MediaMeta(QObject* parent)
: QObject(parent)
, m_localPath{new Rd::Library::LocalPath} {
    connect(&m_mimeWatcher, &QFutureWatcher<QVariantMap>::finished, this, [this]() {
        QVariantMap result = m_mimeWatcher.result();
        Q_EMIT mimeSorted(result["videos"].value<QList<QUrl>>(), result["subtitles"].value<QMap<QString, QUrl>>());
    });

    connect(&m_metaWatcher, &QFutureWatcher<QList<File>>::finished, this, [this]() {
        QList<File> result = m_metaWatcher.result();
        Q_EMIT filesMeta(result);
    });
}

Rd::Library::MediaMeta::~MediaMeta() noexcept {
    m_mimeWatcher.cancel();
    m_mimeWatcher.waitForFinished();

    m_metaWatcher.cancel();
    m_metaWatcher.waitForFinished();

    m_localPath->deleteLater();
}

void Rd::Library::MediaMeta::mimeSort(const QList<QUrl>& urls) {
    if (m_mimeWatcher.isRunning()) {
        Q_EMIT error("Error getting file mime types", "Attempting to run multiple mime sorts");
        return;
    }

    QFuture<QVariantMap> future = QtConcurrent::run([this, urls]() {
        return this->doMimeSort(urls);
    });
    m_mimeWatcher.setFuture(future);
}

void Rd::Library::MediaMeta::getFilesMeta(const QList<QUrl>& urls, const QMap<QString, QUrl>& subtitles) {
    if (m_metaWatcher.isRunning()) {
        Q_EMIT error("Error getting file meta data", "Attempting to run multiple meta data fetchers");
        return;
    }

    QFuture<QList<File>> future = QtConcurrent::run([this, urls, subtitles] {
        return this->doGetFilesMeta(urls, subtitles);
    });
    m_metaWatcher.setFuture(future);
}

QVariantMap Rd::Library::MediaMeta::doMimeSort(const QList<QUrl>& urls) {
    QList<QUrl> videos;
    QMap<QString, QUrl> subtitles;
    QMap<QString, QMap<QString, QUrl>> microdvd;
    QMimeDatabase mime;

    for(const QUrl& url : urls) {
        auto info = QFileInfo(url.path());
        if (m_localPath->isLocalOrSmb(url)) {
            QMimeType type = mime.mimeTypeForUrl(url);
            if(type.name().startsWith(u"video/")) {
                videos << url;
            } else if (type.inherits("application/x-subrip")) {
                subtitles.insert(info.completeBaseName(), url);
            } else if (type.inherits("application/octet-stream") && info.suffix() == u"idx"_qs) {
                if (microdvd.contains(info.completeBaseName())) {
                    microdvd[info.completeBaseName()].insert(info.suffix(), url);
                } else {
                    QMap<QString, QUrl> item;
                    item.insert(info.suffix(), url);
                    microdvd.insert(info.completeBaseName(), item);
                }
            } else if (type.inherits("text/x-microdvd")) {
                if (microdvd.contains(info.completeBaseName())) {
                    microdvd[info.completeBaseName()].insert(info.suffix(), url);
                } else {
                    QMap<QString, QUrl> item;
                    item.insert(info.suffix(), url);
                    microdvd.insert(info.completeBaseName(), item);
                }
            } else {
                qDebug() << "Unhandled file type" << type.name();
            }
        }
    }

    for (auto key : microdvd.keys()) {
        auto values = microdvd[key];
        if (values.contains(u"idx"_qs) && values.contains(u"sub"_qs)) {
            subtitles.insert(key, values.value(u"idx"_qs));
        }
    }

    return QVariantMap({
        {"videos", QVariant::fromValue(videos)},
        {"subtitles", QVariant::fromValue(subtitles)}
    });
}

QList<File> Rd::Library::MediaMeta::doGetFilesMeta(const QList<QUrl>& urls, const QMap<QString, QUrl>& subtitles) {
    QList<File> files;

    for(const QUrl& url : urls) {
        File file = getFileMeta(url);
        if (!file.isEmpty()) {
            QString name = QFileInfo(url.fileName()).completeBaseName();
            if (subtitles.contains(name)) {
                file.selectedSubtitle = subtitles[name];
            }
            files << file;
        }
    }

    return files;
}

File Rd::Library::MediaMeta::getFileMeta(const QUrl& url) {
    QString path = m_localPath->getLocalPath(url);

    AVFormatContext* fmt_ctx = nullptr;
    int err = avformat_open_input(&fmt_ctx, path.toLatin1(), nullptr, nullptr);
    if (err != 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {0};
        av_strerror(err, errbuf, sizeof(errbuf));
        Q_EMIT error("Unable to get meta data", QString::fromUtf8(errbuf));
        return File();
    }

    err = avformat_find_stream_info(fmt_ctx, nullptr);
    if (err < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {0};
        av_strerror(err, errbuf, sizeof(errbuf));
        Q_EMIT error("Unable to get meta data", QString::fromUtf8(errbuf));
        return File();
    }

    File file(url.toString());
    file.runtime = (fmt_ctx->duration + ((AV_TIME_BASE * 60) - 1)) / (AV_TIME_BASE * 60);

    AVDictionaryEntry* metaTag = nullptr;
    while ((metaTag = av_dict_get(fmt_ctx->metadata, "", metaTag, AV_DICT_IGNORE_SUFFIX))) {
        file.meta.insert(QString::fromUtf8(metaTag->key), QString::fromUtf8(metaTag->value));
    }

    for (unsigned int i = 0; i < fmt_ctx->nb_streams; ++i) {
        AVStream* stream = fmt_ctx->streams[i];
        AVCodecParameters* codecpar = stream->codecpar;
        const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
        QString codecName = codec ? QString::fromUtf8(codec->name) : "unknown";

        QVariantMap streamMeta;
        AVDictionaryEntry* streamMetaTag = nullptr;
        while ((streamMetaTag = av_dict_get(stream->metadata, "", streamMetaTag, AV_DICT_IGNORE_SUFFIX))) {
            streamMeta.insert(QString::fromUtf8(streamMetaTag->key), QString::fromUtf8(streamMetaTag->value));
        }
        streamMeta.insert("codec", codecName);

        QStringList dispositionNames;
        if (stream->disposition & AV_DISPOSITION_DEFAULT) dispositionNames << "default";
        if (stream->disposition & AV_DISPOSITION_DUB) dispositionNames << "dub";
        if (stream->disposition & AV_DISPOSITION_ORIGINAL) dispositionNames << "original";
        if (stream->disposition & AV_DISPOSITION_COMMENT) dispositionNames << "comment";
        if (stream->disposition & AV_DISPOSITION_LYRICS) dispositionNames << "lyrics";
        if (stream->disposition & AV_DISPOSITION_KARAOKE) dispositionNames << "karaoke";
        if (stream->disposition & AV_DISPOSITION_FORCED) dispositionNames << "forced";
        if (stream->disposition & AV_DISPOSITION_HEARING_IMPAIRED) dispositionNames << "hearing_impaired";
        if (stream->disposition & AV_DISPOSITION_VISUAL_IMPAIRED) dispositionNames << "visual_impaired";
        if (stream->disposition & AV_DISPOSITION_CLEAN_EFFECTS) dispositionNames << "clean_effects";
        if (stream->disposition & AV_DISPOSITION_ATTACHED_PIC) dispositionNames << "attached_pic";
        if (stream->disposition & AV_DISPOSITION_TIMED_THUMBNAILS) dispositionNames << "timed_thumbnails";

        streamMeta.insert("disposition", dispositionNames.join(", "));

        switch (codecpar->codec_type) {
            case AVMEDIA_TYPE_VIDEO:
                streamMeta.insert("resolution", resolution(codecpar->width, codecpar->height));
                file.video.append(streamMeta);
                break;
            case AVMEDIA_TYPE_AUDIO:
                file.audio.append(streamMeta);
                break;
            case AVMEDIA_TYPE_SUBTITLE:
                file.subtitles.append(streamMeta);
                break;
            default:
                continue;
        }
    }

    avformat_close_input(&fmt_ctx);

    return file;
}
