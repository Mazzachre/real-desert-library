#include "ui.h"
#include "busy-handler.h"
#include "mode-handler.h"
#include "file-handler.h"
#include "sort-order-wrapper.h"
#include "people/people-search.h"

Rd::Ui::Ui::Ui(QObject* parent)
: QObject(parent)
, m_engine{new QQmlApplicationEngine}
, m_shows{new Rd::Ui::Shows::Shows}
, m_show{new Rd::Ui::Show::Show}
, m_movies{new Rd::Ui::Movies::Movies}
, m_movie{new Rd::Ui::Movie::Movie} {
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "MainUI", this);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "BusyIndication", BusyHandler::instance());
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "ModeHandler", ModeHandler::instance());
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "FileHandler", FileHandler::instance());
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "ShowsUI", m_shows);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "ShowUI", m_show);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "MoviesUI", m_movies);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "MovieUI", m_movie);
    qmlRegisterUncreatableType<Ui>("com.realdesert", 1, 0, "Mode", "Uncreateable enum type");
    qmlRegisterUncreatableType<SortOrderWrapper>("com.realdesert", 1, 0, "SortOrder", "Uncreateable enum type");
    qmlRegisterType<People::PeopleSearch>("com.realdesert", 1, 0, "PersonSearch");

    m_engine->addImportPath("qrc:/");
    m_engine->load(QUrl("qrc:/com/realdesert/ui/qml/main.qml"));
    if (m_engine->rootObjects().isEmpty()) qFatal("Unable to parse QML file");
    m_window = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first());
    if (!m_window) qFatal("No root window created");
    ((QWindow *)m_window)->setIcon(QIcon(":/com/realdesert/ui/images/desert-logo.svg"));

    m_loader = m_window->findChild<QObject*>("mainLoader");
    if (!m_loader) qFatal("Loader not found!");

    connect(FileHandler::instance(), &FileHandler::error, this, &Ui::handleError);
    connect(FileHandler::instance(), &FileHandler::warn, this, &Ui::handleWarning);

    connect(m_shows, &Rd::Ui::Shows::Shows::error, this, &Ui::handleError);
    connect(m_show, &Rd::Ui::Show::Show::error, this, &Ui::handleError);
    connect(m_movies, &Rd::Ui::Movies::Movies::error, this, &Ui::handleError);
    connect(m_movie, &Rd::Ui::Movie::Movie::error, this, &Ui::handleError);

    connect(m_shows, &Rd::Ui::Shows::Shows::showSelected, m_show, &Show::Show::loadShow);
    connect(m_show, &Rd::Ui::Show::Show::showLoaded, this, []() {
        ModeHandler::instance()->setMode(ModeHandler::Show);
    });
    connect(m_movies, &Rd::Ui::Movies::Movies::movieSelected, m_movie, &Movie::Movie::loadMovie);
    connect(m_movie, &Rd::Ui::Movie::Movie::movieLoaded, this, []() {
        ModeHandler::instance()->setMode(ModeHandler::Movie);
    });

    connect(ModeHandler::instance(), &ModeHandler::modeUpdated, this, &Ui::handleModeChange);
}

Rd::Ui::Ui::~Ui() noexcept {
    m_shows->deleteLater();
    m_show->deleteLater();
    m_movies->deleteLater();
    m_movie->deleteLater();
    delete m_engine;
}

void Rd::Ui::Ui::show(const QRect& dimensions) {
    m_window->setHeight(dimensions.height()/2);
    m_window->setWidth(dimensions.width()/2);
    m_window->setMinimumHeight(dimensions.height()/2);
    m_window->setMinimumWidth(dimensions.width()/2);
    ModeHandler::instance()->setMode(ModeHandler::Shows);
    m_window->show();
}

void Rd::Ui::Ui::handleModeChange() {
    if (m_loader) {
        m_loader->setProperty("active", false);
        m_loader->setProperty("source", "");

        if (ModeHandler::instance()->mode() != ModeHandler::Shows) m_shows->clear();
        if (ModeHandler::instance()->mode() != ModeHandler::Show) m_show->clear();
        if (ModeHandler::instance()->mode() != ModeHandler::Movies) m_movies->clear();
        if (ModeHandler::instance()->mode() != ModeHandler::Movie) m_movie->clear();

        switch(ModeHandler::instance()->mode()) {
            case ModeHandler::Shows:
                m_shows->init();
                m_loader->setProperty("source", "qrc:/com/realdesert/ui/qml/pages/Shows.qml");
                break;
            case ModeHandler::Show:
                m_loader->setProperty("source", "qrc:/com/realdesert/ui/qml/pages/Show.qml");
                break;
            case ModeHandler::Movies:
                m_movies->init();
                m_loader->setProperty("source", "qrc:/com/realdesert/ui/qml/pages/Movies.qml");
                break;
            case ModeHandler::Movie:
                m_loader->setProperty("source", "qrc:/com/realdesert/ui/qml/pages/Movie.qml");
                break;
            default:
                m_loader->setProperty("source", "qrc:/com/realdesert/ui/qml/pages/About.qml");
        }

        m_loader->setProperty("active", true);
    }
}

void Rd::Ui::Ui::handleError(const QString& headline, const QString& errorText) {
    Rd::Ui::BusyHandler::instance()->setBusy(false);
    qDebug() << "error" << headline << errorText;
}

void Rd::Ui::Ui::handleWarning(const QString& text) {
    qDebug() << "warning" << text;
}
