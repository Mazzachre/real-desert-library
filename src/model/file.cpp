#include "file.h"

File::File() {
    id = 0;
}

File::File(QString path) {
    id = 0;
    this->path = path;
}

bool File::isCreated() {
    return id != 0;
}

bool File::isEmpty() {
    return path.isEmpty();
}

QDebug operator<<(QDebug dbg, const File& data) {
    dbg.nospace().noquote() << "File:(" << data.id << " " << data.path << " " << data.selectedSubtitle << " " << data.runtime << " meta: <" <<  data.meta << "> video: <" << data.video << "> audio: <" << data.audio << "> subtitles: <" << data.subtitles << ">)";
    return dbg;
}
