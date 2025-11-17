#include "imdb-parser.h"
#include <QTextStream>
#include <QDebug>
#include <QThread>

Rd::Library::ImdbParser::ImdbParser(QObject* parent)
: QObject(parent) {
}

Rd::Library::ImdbParser::~ImdbParser() noexcept {
}

void Rd::Library::ImdbParser::parse(QTemporaryFile* file) {
    m_running = true;
    if (!file->isOpen()) {
        if (!file->open()) {
            Q_EMIT error("Could not open IMDb file");
            delete file;
            return;
        }
    }

    quint64 size = file->size();
    const QSet<QString> allowedTypes = { "movie", "tvSeries", "tvMiniSeries", "tvMovie", "video" };

    QList<ImdbGenres> lines;
    QTextStream in(file);
    if (!in.atEnd()) {
        in.readLine();
    }
    while (m_running && !in.atEnd()) {
        const QString line = in.readLine();
        QStringList fields = line.split('\t');

        if (fields.count() == 9 && fields[4] == "0" && fields[8] != "\\N" && allowedTypes.contains(fields[1])) {
            ImdbGenres genres;
            genres.id = fields[0];
            genres.genres = fields[8].split(',');
            lines.append(genres);

            if (m_running && lines.count() >= 100) {
                Q_EMIT parsed(lines);
                lines.clear();
                Q_EMIT progress(file->pos(), size);
            }
        }
    }
    if (m_running && !lines.isEmpty()) {
        Q_EMIT parsed(lines);
    }

    file->close();
    delete file;
    Q_EMIT finished();
}
