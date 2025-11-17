#pragma once

#include <QObject>
#include <QTemporaryFile>
#include <QList>
#include "../model/imdb-genres.h"

namespace Rd {
    namespace Library {
        class ImdbParser : public QObject {
            Q_OBJECT
        public:
            explicit ImdbParser(QObject* parent = nullptr);
            ~ImdbParser() noexcept;

            Q_SLOT void parse(QTemporaryFile* file);
            Q_SIGNAL void parsed(const QList<ImdbGenres>& lines);
            Q_SIGNAL void progress(quint64 read, quint64 total);
            Q_SIGNAL void finished();
            Q_SIGNAL void error(const QString& text);
        private:
            bool m_running;
        };
    }
}
