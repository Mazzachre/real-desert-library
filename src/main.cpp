#include <QApplication>
#include "app/application.h"
#include <QString>
#include <QScreen>
#include <QTimer>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(u"Real Desert Library"_qs);
    app.setApplicationVersion(u"0.1"_qs);
    app.setOrganizationDomain(u"realdesert.com"_qs);
    app.setOrganizationName(u"Real Desert Productions"_qs);

    Rd::Application::Application application(&app);

    QTimer::singleShot(0, [&application]() {
        application.start();
    });

    return app.exec();
}
