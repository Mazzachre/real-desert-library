#pragma once

#include <QAbstractListModel>
#include <QHash>

namespace Rd {
    namespace Ui {
        class Warnings : public QAbstractListModel {
            Q_OBJECT
        public:
            explicit Warnings(QObject* parent = nullptr);
            ~Warnings() noexcept;

            enum RoleList {
                TextRole = Qt::UserRole + 1,
                TimeRole
            };

            Q_SLOT void handleWarning(const QString& text);
            Q_INVOKABLE void close(int index);

            QHash<int, QByteArray> roleNames() const override;
            int rowCount(const QModelIndex &parent = QModelIndex()) const override;
            QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        private:
            QVariantList m_warnings;
        };
    }
}
