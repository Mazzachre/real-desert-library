#include "application.h"
#include "model/file.h"
#include "model/playback.h"
#include "model/imdb-genres.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

Rd::Application::Application::Application(QApplication* app, QObject* parent)
: QObject(parent)
, m_db{new Init::Database}
, m_imdb{new Init::Imdb}
, m_ui{new Ui::Ui} {
    m_dimensions = app->primaryScreen()->geometry();
    connect(app, &QApplication::aboutToQuit, this, &Application::close);
    connect(m_ui, &Ui::Ui::error, this, &Application::handleError);

    connect(m_db, &Init::Database::progress, this, &Application::splashMessage);
    connect(m_imdb, &Init::Imdb::progress, this, &Application::splashMessage);

    connect(m_db, &Init::Database::error, this, &Application::handleError);
    connect(m_imdb, &Init::Imdb::error, this, &Application::handleError);

    connect(m_db, &Init::Database::done, m_imdb, &Init::Imdb::start);
    connect(m_imdb, &Init::Imdb::done, this, &Application::onInitDone);
}

Rd::Application::Application::~Application() noexcept {
    delete m_ui;
    delete m_imdb;
    delete m_db;
}

void Rd::Application::Application::start() {
    showSplash();
    qRegisterMetaType<File>("File");
    qRegisterMetaType<ImdbGenres>("ImdbGenres");
    qRegisterMetaType<Playback>("Playback");

    QTimer::singleShot(0, this, [this]() {
        m_db->start();
    });
}

void Rd::Application::Application::close() {
    if (QSqlDatabase::database().isOpen()) {
        QSqlDatabase::database().close();
    }
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void Rd::Application::Application::handleError(const QString& text) {
    delete m_ui;
    delete m_imdb;
    delete m_db;

    hideSplash();
    QMessageBox::critical(nullptr, "Initialization Error", text);
    qApp->exit(1);
}

void Rd::Application::Application::onInitDone() {
    hideSplash();
    m_ui->show(m_dimensions);
    m_ui->setVisibility(true);
}

void Rd::Application::Application::showSplash() {
    qDebug() << "Show Splash";
}

void Rd::Application::Application::splashMessage(const QString& text) {
    qDebug() << "Msg:" << text;
}

void Rd::Application::Application::hideSplash() {
    qDebug() << "Hide Splash";
}
