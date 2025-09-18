#include "application.h"
#include "../model/file.h"
#include <QDebug>
#include <QMessageBox>

Rd::Application::Application::Application(const QRect& dimensions, QObject* parent)
: QObject(parent)
, m_db{new Rd::Database::Database}
, m_ui{new Rd::Ui::Ui} {
    m_dimensions = dimensions;
}

Rd::Application::Application::~Application() noexcept {
    delete m_ui;
    delete m_db;
}

bool Rd::Application::Application::init() {
    showSplash();
    auto splashGuard = qScopeGuard([this] { hideSplash(); });

    qRegisterMetaType<File>("File");

    QSqlError openError = m_db->open();
    if (openError.type() != QSqlError::NoError) {
        QMessageBox::critical(nullptr, "Error opening database", openError.text());
        return false;
    }
    QSqlError migrateError = m_db->migrate();
    if (migrateError.type() != QSqlError::NoError) {
        QMessageBox::critical(nullptr, "Error migrating database", migrateError.text());
        return false;
    }

    return true;
}

void Rd::Application::Application::run() {
    m_ui->show(m_dimensions);
}

void Rd::Application::Application::close() {
    m_db->close();
}

void Rd::Application::Application::showSplash() {
    qDebug() << "Splash";
}

void Rd::Application::Application::hideSplash() {
    qDebug() << "Splish";
}
