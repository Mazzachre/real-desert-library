#include "episode-details.h"

Rd::Ui::Show::EpisodeDetails::EpisodeDetails(QObject* parent)
: QObject(parent) {
}

Rd::Ui::Show::EpisodeDetails::~EpisodeDetails() noexcept {
}

void Rd::Ui::Show::EpisodeDetails::setEpisode(const EpisodeListItem& episode) {
    m_episode = episode;
    Q_EMIT dataUpdated();
    Q_EMIT open();
}

void Rd::Ui::Show::EpisodeDetails::setFavorite() {
    m_episode.favorite = !m_episode.favorite;
    Q_EMIT favoriteSet(m_episode.id, m_episode.favorite);
    Q_EMIT dataUpdated();
}

QString Rd::Ui::Show::EpisodeDetails::name() {
    return m_episode.name;
}

bool Rd::Ui::Show::EpisodeDetails::favorite() {
    return m_episode.favorite;
}

QVariantList mapToList(const QVariantMap& map) {
    QVariantList list;
    for (auto it = map.begin(); it != map.end(); ++it) {
        QVariantMap entry;
        entry["key"] = it.key();
        entry["value"] = it.value();
        list.append(QVariant(entry));
    }
    return list;
}

QVariantList Rd::Ui::Show::EpisodeDetails::episode() {
    QVariantList data;
    data.append(QVariantMap({
        {"key", "Original Air Date"},
        {"value", m_episode.airDate.toString()}
    }));
    data.append(QVariantMap({
        {"key", "Overview"},
        {"value", m_episode.overview}
    }));
    data.append(QVariantMap({
        {"key", "Runtime (Actual)"},
        {"value", QString("%1 (%2)").arg(m_episode.officialRuntime).arg(m_episode.actualRuntime)}
    }));
    return data;
}

QVariantList Rd::Ui::Show::EpisodeDetails::file() {
    auto meta = mapToList(m_episode.meta);
    if (!m_episode.path.isEmpty()) {
        meta.append(QVariantMap({
            {"key", "Path"},
            {"value", m_episode.path}
        }));
    }
    return meta;
}

QVariantList Rd::Ui::Show::EpisodeDetails::video() {
    return mapToList(m_episode.video);
}

QVariantList Rd::Ui::Show::EpisodeDetails::audio() {
    return mapToList(m_episode.audio);
}

QVariantList Rd::Ui::Show::EpisodeDetails::subtitle() {
    return mapToList(m_episode.subtitles);
}
