#pragma once

#include <QAbstractListModel>

namespace Rd {
    namespace Ui {
        namespace Shows {
            class ShowTagsList : public QAbstractListModel {
                Q_OBJECT
            public:
                enum GenreRoles {
                    IdRole = Qt::UserRole + 1,
                    NameRole,
                    SelectedRole,
                };

                void setTags(const QMap<quint16, QString>& tags);
                void clear();

                QList<quint16> selected();

                Q_INVOKABLE void toggle(quint16 id);
                Q_SIGNAL void selectedUpdated();

                explicit ShowTagsList(QObject* parent = nullptr);
                ~ShowTagsList() noexcept;

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
            private:
                QList<QVariantMap> m_data;
            };
        }
    }
}

