#pragma once

#include <QString>

namespace Rd {
    namespace Library {
        inline QString formatRuntime(int minutes) {
            return QString("%1:%2")
                .arg(minutes / 60)
                .arg(minutes % 60, 2, 10, QChar('0'));
        }
    }
}
