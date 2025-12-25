#pragma once

#include <QObject>

namespace Rd {
    namespace Ui {
        namespace Movie {
            class Movie : public QObject {
                Q_OBJECT
            public:
                explicit Movie(QObject* parent = nullptr);
                ~Movie() noexcept;

                Q_SLOT void load(quint32 id);
                void clear();
                Q_SIGNAL void movieLoaded();

                Q_SIGNAL void error(const QString& header, const QString& body);
                Q_SIGNAL void warn(const QString& text);
            };
        }
    }
}
