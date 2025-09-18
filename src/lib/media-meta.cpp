#include "media-meta.h"
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
: QObject(parent) {
}

Rd::Library::MediaMeta::~MediaMeta() noexcept {
}

QSqlError Rd::Library::MediaMeta::getFileMeta(File& file) {
    if (file.path.isEmpty()) {
        return QSqlError("Unable to get meta data", "No path given to get meta data");
    }

    AVFormatContext* fmt_ctx = nullptr;
    int err = avformat_open_input(&fmt_ctx, file.path.toLatin1(), nullptr, nullptr);
    if (err != 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {0};
        av_strerror(err, errbuf, sizeof(errbuf));
        return QSqlError("Unable to get meta data", QString::fromUtf8(errbuf));
    }

    err = avformat_find_stream_info(fmt_ctx, nullptr);
    if (err < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {0};
        av_strerror(err, errbuf, sizeof(errbuf));
        return QSqlError("Unable to get meta data", QString::fromUtf8(errbuf));
    }

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

    return QSqlError();
}
