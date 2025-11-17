#include "show-search-list.h"
#include "ui/busy-handler.h"
#include <QJsonObject>
#include <QJsonArray>

Rd::Ui::Shows::ShowSearchList::ShowSearchList(QObject* parent)
: QAbstractListModel(parent)
, m_show{new Rd::Net::Tmdb::Show} {
    connect(m_show, &Rd::Net::Tmdb::Show::searchResult, this, &ShowSearchList::setResults);
    connect(m_show, &Rd::Net::Tmdb::Show::error, this, [this](const QString& text) {
        emit error("TMDB Error", text);
    });
}

Rd::Ui::Shows::ShowSearchList::~ShowSearchList() noexcept {
    m_show->deleteLater();
}

void Rd::Ui::Shows::ShowSearchList::search(const QString& name, quint16 page) {
    m_show->search(name, page);
    Rd::Ui::BusyHandler::instance()->setBusy(true);
}

void Rd::Ui::Shows::ShowSearchList::clear() {
    beginResetModel();
    m_results.clear();
    m_page = 0;
    m_total = 0;
    endResetModel();
    Q_EMIT resultsUpdated();
}

bool Rd::Ui::Shows::ShowSearchList::useOriginalName() const {
    return m_useOriginalName;
}

void Rd::Ui::Shows::ShowSearchList::setUseOriginalName(bool useOriginalName) {
    m_useOriginalName = useOriginalName;
    Q_EMIT useOriginalNameChanged();
}

int Rd::Ui::Shows::ShowSearchList::page() const {
    return m_page;
}

int Rd::Ui::Shows::ShowSearchList::total() const {
    return m_total;
}

QHash<int, QByteArray> Rd::Ui::Shows::ShowSearchList::roleNames() const {
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {OriginalLanguageRole, "originalLanguage"},
        {OriginalNameRole, "originalName"},
        {OverviewRole, "overview"},
        {PopularityRole, "popularity"},
        {PosterPathRole, "posterPath"},
        {ReleaseYearRole, "releaseYear"}
    };
}

int Rd::Ui::Shows::ShowSearchList::rowCount(const QModelIndex& parent) const {
    return m_results.count();
}

QVariant Rd::Ui::Shows::ShowSearchList::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        const ShowSearchResult& result = m_results[index.row()];
        switch(role) {
            case IdRole:
                return result.id;
            case NameRole:
                return result.name;
            case OriginalLanguageRole:
                return result.originalLanguage;
            case OriginalNameRole:
                return result.originalName;
            case ReleaseYearRole:
                return result.firstAirDate.isValid() ? QVariant(result.firstAirDate.year()) : QVariant(u"Unknown"_qs);
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

void Rd::Ui::Shows::ShowSearchList::setResults(const QJsonObject& result) {
    beginResetModel();
    m_results.clear();
    m_page = result[u"page"].toInt();
    m_total = result[u"total_pages"].toInt();

    QJsonArray results = result[u"results"].toArray();
    for (const QJsonValue& item : results) {
        m_results << ShowSearchResult(item.toObject());
    }
    endResetModel();
    Rd::Ui::BusyHandler::instance()->setBusy(false);
    Q_EMIT resultsUpdated();
}
