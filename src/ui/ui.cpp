#include "ui.h"
#include "busy-handler.h"
#include "mode-handler.h"
#include "file-handler.h"
#include "../enums/extras-type.h"
#include "../enums/sort-order.h"

Rd::Ui::Ui::Ui(QObject* parent)
: QObject(parent)
, m_engine{new QQmlApplicationEngine}
, m_shows{new Shows::Shows}
, m_show{new Show::Show}
, m_movies{new Movies::Movies}
, m_movie{new Movie::Movie}
, m_extras{new Extras::Extras}
, m_person{new People::PersonSearch}
, m_warnings{new Warnings}
, m_blocker{new DialogBlocker} {
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "MainUI", this);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "BusyIndication", BusyHandler::instance());
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "ModeHandler", ModeHandler::instance());
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "FileHandler", FileHandler::instance());
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "ShowsUI", m_shows);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "ShowUI", m_show);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "MoviesUI", m_movies);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "MovieUI", m_movie);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "ExtrasUI", m_extras);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "PersonSearch", m_person);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "Warnings", m_warnings);
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "Blocker", m_blocker);

    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "SortOrder", Rd::Enums::SortOrder::instance());
    qmlRegisterSingletonInstance("com.realdesert", 1, 0, "ExtrasType", Rd::Enums::ExtrasType::instance());


    m_engine->addImportPath("qrc:/");
    m_engine->load(QUrl("qrc:/com/realdesert/ui/qml/main.qml"));
    if (m_engine->rootObjects().isEmpty()) qFatal("Unable to parse QML file");
    m_window = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first());
    if (!m_window) qFatal("No root window created");
    ((QWindow *)m_window)->setIcon(QIcon(":/com/realdesert/ui/images/desert-logo.svg"));

    m_loader = m_window->findChild<QObject*>("mainLoader");
    if (!m_loader) qFatal("Loader not found!");

    connect(FileHandler::instance(), &FileHandler::error, this, &Ui::handleError);
    connect(m_shows, &Shows::Shows::error, this, &Ui::handleError);

    connect(FileHandler::instance(), &FileHandler::warn, m_warnings, &Warnings::handleWarning);
    connect(m_shows, &Shows::Shows::warn, m_warnings, &Warnings::handleWarning);
    connect(m_show, &Show::Show::warn, m_warnings, &Warnings::handleWarning);
    connect(m_extras, &Extras::Extras::warn, m_warnings, &Warnings::handleWarning);

    connect(m_show, &Show::Show::error, this, &Ui::handleError);

    connect(m_movies, &Movies::Movies::error, this, &Ui::handleError);
    connect(m_movie, &Movie::Movie::error, this, &Ui::handleError);

    connect(m_extras, &Extras::Extras::error, this, &Ui::handleError);

    connect(m_shows, &Shows::Shows::showSelected, m_show, &Show::Show::load);
    connect(m_show, &Show::Show::showLoaded, this, []() {
        ModeHandler::instance()->setMode(ModeHandler::Show);
    });
    connect(m_movies, &Movies::Movies::movieSelected, m_movie, &Movie::Movie::loadMovie);
    connect(m_movie, &Movie::Movie::movieLoaded, this, []() {
        ModeHandler::instance()->setMode(ModeHandler::Movie);
    });
    connect(m_extras, &Extras::Extras::extrasLoaded, this, []() {
        ModeHandler::instance()->setMode(ModeHandler::Extras);
    });
    connect(m_extras, &Extras::Extras::goBack, this, [this](Enums::ExtrasType::Type type, quint32 id) {
        switch(type) {
            case Enums::ExtrasType::Type::Movie:
                m_movie->loadMovie(id);
                break;
            case Enums::ExtrasType::Type::Show:
                m_show->load(id);
                break;
            case Enums::ExtrasType::Type::Episode:
                m_show->loadByEpisodeId(id);
                break;
        }
    });

    connect(ModeHandler::instance(), &ModeHandler::modeUpdated, this, &Ui::handleModeChange);
}

Rd::Ui::Ui::~Ui() noexcept {
    m_shows->deleteLater();
    m_show->deleteLater();
    m_movies->deleteLater();
    m_movie->deleteLater();
    m_extras->deleteLater();
    m_person->deleteLater();
    m_warnings->deleteLater();
    m_blocker->deleteLater();
    delete m_engine;
}

void Rd::Ui::Ui::show(const QRect& dimensions) {
    m_window->setHeight(dimensions.height()/2);
    m_window->setWidth(dimensions.width()/2);
    m_window->setMinimumHeight(std::min(600, dimensions.height()));
    m_window->setMinimumWidth(std::min(900, dimensions.width()));
    ModeHandler::instance()->setMode(ModeHandler::Shows);
}

void Rd::Ui::Ui::setVisibility(bool visible) {
    if (visible) {
        m_window->show();
    } else {
        m_window->hide();
    }
}


void Rd::Ui::Ui::handleModeChange() {
    if (m_loader) {
        m_loader->setProperty("active", false);
        m_loader->setProperty("source", "");

        if (ModeHandler::instance()->mode() != ModeHandler::Shows) m_shows->clear();
        if (ModeHandler::instance()->mode() != ModeHandler::Show) m_show->clear();
        if (ModeHandler::instance()->mode() != ModeHandler::Movies) m_movies->clear();
        if (ModeHandler::instance()->mode() != ModeHandler::Movie) m_movie->clear();
        if (ModeHandler::instance()->mode() != ModeHandler::Extras) m_extras->clear();
        m_person->clear();
        m_blocker->reset();

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
            case ModeHandler::Extras:
                m_loader->setProperty("source", "qrc:/com/realdesert/ui/qml/pages/Extras.qml");
                break;
            default:
                m_loader->setProperty("source", "qrc:/com/realdesert/ui/qml/pages/About.qml");
        }

        m_loader->setProperty("active", true);
    }
}

void Rd::Ui::Ui::handleError(const QString& headline, const QString& errorText) {
    Rd::Ui::BusyHandler::instance()->setBusy(false);
    Q_EMIT error(headline + ": " + errorText);
}
