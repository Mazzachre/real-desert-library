#include "movie-search-list.h"

#include <QJsonObject>
#include <QJsonArray>

Rd::Ui::Movies::MovieSearchList::MovieSearchList(QObject* parent)
: QAbstractListModel(parent)
, m_movie{new Rd::Net::Tmdb::Movie} {
    connect(m_movie, &Rd::Net::Tmdb::Movie::searchResult, this, &MovieSearchList::setResults);
    connect(m_movie, &Rd::Net::Tmdb::Movie::error, this, [this](const QString& header, const QString& body) {
        emit error(header, body);
    });
}

Rd::Ui::Movies::MovieSearchList::~MovieSearchList() noexcept {
    m_movie->deleteLater();
}


void Rd::Ui::Movies::MovieSearchList::search(const QString& name, quint16 page) {
    m_movie->search(name, page);
}

void Rd::Ui::Movies::MovieSearchList::clear() {
    beginResetModel();
    m_results.clear();
    m_page = 0;
    m_total = 0;
    endResetModel();
    Q_EMIT resultsUpdated();
}

bool Rd::Ui::Movies::MovieSearchList::useOriginalTitle() const {
    return m_useOriginalTitle;
}

void Rd::Ui::Movies::MovieSearchList::setUseOriginalTitle(bool useOriginalTitle) {
    m_useOriginalTitle = useOriginalTitle;
    Q_EMIT useOriginalTitleChanged();
}

int Rd::Ui::Movies::MovieSearchList::page() const {
    return m_page;
}

int Rd::Ui::Movies::MovieSearchList::total() const {
    return m_total;
}

QHash<int, QByteArray> Rd::Ui::Movies::MovieSearchList::roleNames() const {
    return {
        {IdRole, "id"},
        {TitleRole, "title"},
        {OriginalLanguageRole, "originalLanguage"},
        {OriginalTitleRole, "originalTitle"},
        {OverviewRole, "overview"},
        {PopularityRole, "popularity"},
        {PosterPathRole, "posterPath"},
        {ReleaseYearRole, "releaseYear"}
    };
}

int Rd::Ui::Movies::MovieSearchList::rowCount(const QModelIndex& parent) const {
    return m_results.count();
}

QVariant Rd::Ui::Movies::MovieSearchList::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        const MovieSearchResult& result = m_results[index.row()];
        switch(role) {
            case IdRole:
                return result.id;
            case TitleRole:
                return result.title;
            case OriginalLanguageRole:
                return result.originalLanguage;
            case OriginalTitleRole:
                return result.originalTitle;
            case ReleaseYearRole:
                return result.releaseDate.isValid() ? QVariant(result.releaseDate.year()) : QVariant(u"Unknown"_qs);
            case PopularityRole:
                return result.popularity;
            case OverviewRole:
                return result.overview;
            case PosterPathRole:
                return result.posterPath;
        }
    }
    return QVariant();
}

void Rd::Ui::Movies::MovieSearchList::setResults(const QJsonObject& result) {
    beginResetModel();
    m_results.clear();
    m_page = result["page"].toInt();
    m_total = result["total_pages"].toInt();

    QJsonArray results = result["results"].toArray();
    for (const QJsonValue& item : results) {
        m_results << MovieSearchResult(item.toObject());
    }
    endResetModel();
    Q_EMIT resultsUpdated();
}
