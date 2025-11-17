#pragma once

#include <QObject>

namespace Rd {
    namespace Application {
        namespace Init {
            class Database : public QObject {
                Q_OBJECT
            public:
                explicit Database(QObject* parent = nullptr);
                ~Database() noexcept;

                Q_SLOT void start();
                Q_SIGNAL void done();
                Q_SIGNAL void progress(const QString& text);
                Q_SIGNAL void error(const QString& text);
            };
        }
    }
}

