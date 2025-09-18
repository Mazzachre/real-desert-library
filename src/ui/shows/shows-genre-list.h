#pragma once

#include <QAbstractListModel>

namespace Rd {
    namespace Ui {
        namespace Shows {
            class ShowGenreList : public QAbstractListModel {
                Q_OBJECT
            public:
                enum GenreRoles {
                    IdRole = Qt::UserRole + 1,
                    NameRole,
                    SelectedRole,
                };

                void setGenres(const QMap<quint16, QString>& genres);
                void clear();

                QList<quint16> selected();

                Q_INVOKABLE void toggle(quint16 id);
                Q_SIGNAL void selectedUpdated();

                explicit ShowGenreList(QObject* parent = nullptr);
                ~ShowGenreList() noexcept;

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
            private:
                QList<QVariantMap> m_data;
            };
        }
    }
}
