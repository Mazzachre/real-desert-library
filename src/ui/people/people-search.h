#pragma once

#include <QAbstractListModel>
#include <QList>
#include "../../model/person.h"
#include "../../db/cast-crew.h"

namespace Rd {
    namespace Ui {
        namespace People {
            class PeopleSearch : public QAbstractListModel {
                Q_OBJECT
            public:
                enum RoleList {
                    IdRole = Qt::UserRole + 1,
                    NameRole,
                    OriginalNameRole,
                    ProfilePathRole,
                };

                explicit PeopleSearch(QObject* parent = nullptr);
                ~PeopleSearch() noexcept;

                Q_SLOT void search(const QString& name);
                Q_SLOT void clear();

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

                Q_SIGNAL void error(const QString& header, const QString& body);
            private:
                Rd::Database::CastCrew* m_db;
                QList<Person> m_people;
            };
        }
    }
}
