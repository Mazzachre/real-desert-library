#pragma once

#include <QObject>
#include "db/cast-crew.h"

namespace Rd {
    namespace Ui {
        namespace People {
            class PersonSearch : public QObject {
                Q_OBJECT
                Q_PROPERTY(QVariantList people READ people NOTIFY peopleUpdated)
                Q_PROPERTY(QString selected READ selected WRITE setSelected NOTIFY selectedUpdated)
            public:
                explicit PersonSearch(QObject* parent = nullptr);
                ~PersonSearch() noexcept;

                Q_INVOKABLE void search(const QString& text);
                Q_INVOKABLE void clear();

                QVariantList people();
                Q_SIGNAL void peopleUpdated();

                QString selected();
                void setSelected(const QString& selected);
                Q_SIGNAL void selectedUpdated();

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                Database::CastCrew* m_db;

                QVariantList m_people;
                QString m_selected;
            };
        }
    }
}
