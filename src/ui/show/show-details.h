#pragma once

#include <QObject>
#include "model/show.h"

namespace Rd {
    namespace Ui {
        namespace Show {
            class ShowDetails : public QObject {
                Q_OBJECT
                Q_PROPERTY(quint32 id READ id NOTIFY showUpdated)
                Q_PROPERTY(QString name READ name NOTIFY showUpdated)
                Q_PROPERTY(QString originalName READ originalName NOTIFY showUpdated)
                Q_PROPERTY(bool hasExtras READ hasExtras NOTIFY showUpdated)
                Q_PROPERTY(QVariantList info READ info NOTIFY showUpdated)
                Q_PROPERTY(QVariantList genres READ genres NOTIFY showUpdated)
                Q_PROPERTY(QVariantList tags READ tags NOTIFY showUpdated)
                Q_PROPERTY(QVariantList crew READ crew NOTIFY showUpdated)
                Q_PROPERTY(QVariantList cast READ cast NOTIFY showUpdated)
            public:
                ShowDetails(QObject* parent = nullptr);
                ~ShowDetails() noexcept;

                void setShow(const ::Show& show);
                Q_SIGNAL void showUpdated();
                void clear();

                quint32 id();
                QString name();
                QString originalName();
                bool hasExtras();
                QVariantList info();
                QVariantList genres();
                QVariantList tags();
                QVariantList crew();
                QVariantList cast();
            private:
                ::Show m_show;
            };
        }
    }
}
