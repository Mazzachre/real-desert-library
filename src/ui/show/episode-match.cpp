#include "episode-match.h"
#include "../file-handler.h"
#include <QFileInfo>

Rd::Ui::Show::EpisodeMatch::EpisodeMatch(QObject* parent)
: QAbstractListModel(parent) {
    connect(Rd::Ui::FileHandler::instance(), &Rd::Ui::FileHandler::filesAdded, this, &EpisodeMatch::handleFilesAdded);
}

Rd::Ui::Show::EpisodeMatch::~EpisodeMatch() noexcept {}

QHash<int, QByteArray> Rd::Ui::Show::EpisodeMatch::roleNames() const {
    return {
        {IdRole, "id"},
        {PathRole, "path"},
        {FileRole, "file"},
        {RuntimeRole, "runtime"},
        {MatchRole, "match"},
    };
}

void Rd::Ui::Show::EpisodeMatch::setEpisodes(const QList<EpisodeListItem>& episodes) {
    m_episodes.clear();
    for (const auto& episode : episodes) {
        QVariantMap ep;
        ep["id"] = episode.id;
        QString seasonEpisode = QString("S%1E%2")
            .arg(episode.season, 2, 10, QChar('0'))
            .arg(episode.episode, 2, 10, QChar('0'));
        QString runtime = QString("%1:%2")
            .arg(episode.officialRuntime / 60)
            .arg(episode.officialRuntime % 60, 2, 10, QChar('0'));
        QString name = QString("%1 %2 %3")
            .arg(seasonEpisode)
            .arg(episode.name)
            .arg(runtime);
        ep["name"] = QVariant(name);
        ep["season"] = episode.season;
        ep["episode"] = episode.episode;
        m_episodes.append(ep);
    }
    Q_EMIT updated();
}

void Rd::Ui::Show::EpisodeMatch::clear() {
    beginResetModel();
    m_files.clear();
    m_matches.clear();
    endResetModel();
}

QVariantList Rd::Ui::Show::EpisodeMatch::episodes() {
    return m_episodes;
}

void Rd::Ui::Show::EpisodeMatch::selected(quint32 fileId, quint32 episodeId) {
    m_matches[fileId] = episodeId;
    for (int i = 0; i < m_files.size(); ++i) {
        if (m_files[i].id == fileId) {
            emit dataChanged(index(i), index(i));
            break;
        }
    }
}

void Rd::Ui::Show::EpisodeMatch::save() {
    Q_EMIT episodesMatched(m_matches);
    clear();
}

int Rd::Ui::Show::EpisodeMatch::rowCount(const QModelIndex& parent) const {
    return m_files.count();
}

QVariant Rd::Ui::Show::EpisodeMatch::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        const File& file = m_files[index.row()];
        switch(role) {
            case IdRole:
                return file.id;
            case PathRole:
                return file.path;
            case FileRole:
                return QFileInfo(file.path).fileName();
            case RuntimeRole: {
                QString runtime = QString("%1:%2")
                    .arg(file.runtime / 60)
                    .arg(file.runtime % 60, 2, 10, QChar('0'));
                return runtime;
            }
            case MatchRole:
                return m_matches[file.id];
        }
    }
    return QVariant();
}

void Rd::Ui::Show::EpisodeMatch::handleFilesAdded(const QList<QVariantMap>& files) {
    beginResetModel();
    for (auto& item : files) {
        if (item["type"] == "known" || item["type"] == "created") {
            File file = item["file"].value<File>();
            m_files.append(file);
            quint32 match = getEpisode(findMatch(file.path));
            if (match != 0) {
                m_matches[file.id] = match;
            }
        }
    }
    endResetModel();
    Q_EMIT requestOpenDialog();
}

QVariantMap Rd::Ui::Show::EpisodeMatch::findMatch(const QString& path) {
    static const QRegularExpression regex(R"(s(?<season>\d+)e(?<episode>\d+))", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = regex.match(path);
    if (match.hasMatch()) {
        return {
            {"season", match.captured("season").toULongLong()},
            {"episode", match.captured("episode").toULongLong()}
        };
    }

    return QVariantMap();
}

quint32 Rd::Ui::Show::EpisodeMatch::getEpisode(const QVariantMap& match) {
    for (const auto& episode : m_episodes) {
        if (episode.toMap()["season"] == match["season"] && episode.toMap()["episode"] == match["episode"]) {
            return episode.toMap()["id"].toULongLong();
        }
    }
    return 0;
}

