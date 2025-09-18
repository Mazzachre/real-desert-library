#include <QApplication>
#include "app/application.h"
#include "model/show-search-result.h"
#include "model/person.h"
#include "model/episode.h"
#include "model/show.h"
#include <QString>
#include <QScreen>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(u"Real Desert Library"_qs);
    app.setApplicationVersion(u"0.1"_qs);
    app.setOrganizationDomain(u"realdesert.com"_qs);
    app.setOrganizationName(u"Real Desert Productions"_qs);

    int result = -1;
    {
        Rd::Application::Application application(app.primaryScreen()->geometry());
        if (application.init()) {
            application.run();
            result = app.exec();
        }

        application.close();
    }

    return result;
}
