#include "shows.h"
#include "../../model/show-list-item.h"

Rd::Ui::Shows::Shows::Shows(QObject* parent)
: QObject(parent)
, m_addShow{new Rd::Library::AddShow}
, m_searchResults{new ShowSearchList}
, m_showsList{new ShowsList}
, m_showsFilter{new ShowsFilter}
, m_db{new Rd::Database::Shows} {
    connect(m_searchResults, &ShowSearchList::error, this, [this](const QString& header, const QString& body) {
        Q_EMIT error(header, body);
    });
    connect(m_addShow, &Rd::Library::AddShow::showAdded, this, [this](quint32 id) {
        Q_EMIT showSelected(id);
    });
    connect(m_addShow, &Rd::Library::AddShow::error, this, [this](const QString& header, const QString& body) {
        Q_EMIT error(header, body);
    });
    connect(m_showsFilter, &ShowsFilter::error, this, [this](const QString& header, const QString& body) {
        Q_EMIT error(header, body);
    });
    connect(m_showsFilter, &ShowsFilter::filterUpdated, this, [this]() {
        load();
    });
    m_order = SortOrderWrapper::TitleAsc;
}

Rd::Ui::Shows::Shows::~Shows() noexcept {
    m_db->deleteLater();
    m_showsFilter->deleteLater();
    m_showsList->deleteLater();
    m_searchResults->deleteLater();
}

void Rd::Ui::Shows::Shows::init() {
    m_showsFilter->init();
    Q_EMIT filterUpdated();
}

void Rd::Ui::Shows::Shows::clear() {
    m_searchResults->clear();
    m_showsList->clear();
    m_showsFilter->clear();
}

QAbstractListModel* Rd::Ui::Shows::Shows::searchResults() {
    return m_searchResults;
}

void Rd::Ui::Shows::Shows::addShow(quint32 id) {
    m_searchResults->clear();
    m_addShow->addShow(id);
}

QAbstractListModel* Rd::Ui::Shows::Shows::list() {
    return m_showsList;
}

Rd::Ui::Shows::ShowsFilter* Rd::Ui::Shows::Shows::filter() {
    return m_showsFilter;
}

Rd::Ui::SortOrderWrapper::WrappedSortOrder Rd::Ui::Shows::Shows::order() {
    return m_order;
}

void Rd::Ui::Shows::Shows::setOrder(Rd::Ui::SortOrderWrapper::WrappedSortOrder order) {
    m_order = order;
    load();
    Q_EMIT orderUpdated();
}

void Rd::Ui::Shows::Shows::load() {
    QList<ShowListItem> shows;
    QSqlError err = m_db->findShows(m_showsFilter->filter(), SortOrderWrapper::toSortOrder(m_order), shows);
    if (err.type() != QSqlError::NoError) {
        Q_EMIT error("Error loading show list", err.text());
        return;
    }

    m_showsList->setShows(shows);
    Q_EMIT listUpdated();
}
