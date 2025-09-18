#include "file.h"

File::File() {
    id = 0;
}

File::File(QString path) {
    id = 0;
    //TODO Match fuse files and do something different with them...
    this->path = path;
}

bool File::isCreated() {
    return id != 0;
}

QDebug operator<<(QDebug dbg, const File& data) {
    dbg.nospace().noquote() << "File:(" << data.id << " " << data.path << " " << data.runtime << " meta: <" <<  data.meta << "> video: <" << data.video << "> audio: <" << data.audio << "> subtitles: <" << data.subtitles << ">)";
    return dbg;

}
