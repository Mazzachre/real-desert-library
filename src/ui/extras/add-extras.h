#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QMap>
#include <QHash>
#include <QByteArray>
#include "model/file.h"

namespace Rd {
    namespace Ui {
        namespace Extras {
            class AddExtras : public QAbstractListModel {
                Q_OBJECT
            public:
                enum RoleList {
                    IdRole = Qt::UserRole + 1,
                    FileRole,
                    NameRole,
                    RuntimeRole,
                };

                explicit AddExtras(QObject* parent = nullptr);
                ~AddExtras() noexcept;

                Q_SLOT void save();
                Q_SLOT void clear();
                Q_SLOT void removeFile(quint32 fileId);
                Q_SLOT void updateName(quint32 fileId, const QString& name);

                QHash<int, QByteArray> roleNames() const override;
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

                Q_SIGNAL void requestOpenDialog();
                Q_SIGNAL void extrasAdded(const QMap<quint32, QString>& extras);
            private:
                QList<File> m_files;
                QMap<quint32, QString> m_extras;

                Q_SLOT void handleFilesAdded(const QList<File>& files);
            };
        }
    }
}
