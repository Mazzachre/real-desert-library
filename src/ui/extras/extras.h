#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QHash>
#include <QByteArray>
#include "db/extras.h"
#include "model/extra.h"
#include "enums/extras-type.h"
#include "add-extras.h"

namespace Rd {
    namespace Ui {
        namespace Extras {
            class Extras : public QAbstractListModel {
                Q_OBJECT
                Q_PROPERTY(quint32 selected READ selected WRITE setSelected NOTIFY selectedUpdated)
                Q_PROPERTY(QAbstractListModel* add READ add NOTIFY extrasLoaded)
            public:
                enum RoleList {
                    IdRole = Qt::UserRole + 1,
                    NameRole,
                    RuntimeRole,
                    PlayedRole,
                    PlayedFullyRole,
                    PlaybackListRole,
                };

                Extras(QObject* parent = nullptr);
                ~Extras() noexcept;

                QAbstractListModel* add();

                Q_INVOKABLE void load(Rd::Enums::ExtrasType::Type type, quint32 id);
                Q_INVOKABLE void back();
                void clear();

                quint32 selected();
                void setSelected(quint32 selected);
                Q_SIGNAL void selectedUpdated();

                Q_INVOKABLE void updateName(quint32 id, const QString& name);

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

                Q_SIGNAL void extrasLoaded();
                Q_SIGNAL void goBack(Rd::Enums::ExtrasType::Type type, quint32 id);

                Q_SIGNAL void error(const QString& header, const QString& body);
                Q_SIGNAL void warn(const QString& text);
            private:
                Database::Extras* m_db;
                AddExtras* m_add;
                quint32 m_selected;

                Enums::ExtrasType::Type m_type;
                quint32 m_id;
                QList<ExtraListItem> m_extras;

                void reload();
                Q_SLOT void handleExtrasAdded(const QMap<quint32, QString>& extras);
            };
        }
    }
}
