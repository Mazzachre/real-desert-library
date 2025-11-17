#include "add-extras.h"
#include "ui/file-handler.h"
#include "ui/mode-handler.h"

Rd::Ui::Extras::AddExtras::AddExtras(QObject* parent)
: QAbstractListModel(parent) {
    connect(FileHandler::instance(), &FileHandler::filesAdded, this, &AddExtras::handleFilesAdded);
}

Rd::Ui::Extras::AddExtras::~AddExtras() noexcept {
}

void Rd::Ui::Extras::AddExtras::save() {
    Q_EMIT extrasAdded(m_extras);
    clear();
}

void Rd::Ui::Extras::AddExtras::clear() {
    beginResetModel();
    m_files.clear();
    m_extras.clear();
    endResetModel();
}

void Rd::Ui::Extras::AddExtras::removeFile(quint32 fileId) {
    for (int row = 0; row < m_files.size(); ++row) {
        if (m_files[row].id == fileId) {
            beginRemoveRows(QModelIndex(), row, row);
            m_files.removeAt(row);
            endRemoveRows();
        }
    }
}

void Rd::Ui::Extras::AddExtras::updateName(quint32 fileId, const QString& name) {
    for (int i = 0; i < m_files.count(); ++i) {
        auto& file = m_files[i];
        if (file.id == fileId) {
            m_extras[fileId] = name;
            QModelIndex modelIndex = index(i);
            Q_EMIT dataChanged(modelIndex, modelIndex, {NameRole});
        }
    }
}

QHash<int, QByteArray> Rd::Ui::Extras::AddExtras::roleNames() const {
    return {
        {IdRole, "id"},
        {FileRole, "file"},
        {NameRole, "name"},
        {RuntimeRole, "runtime"},
    };
}

int Rd::Ui::Extras::AddExtras::rowCount(const QModelIndex& parent) const {
    return m_files.count();
}

QVariant Rd::Ui::Extras::AddExtras::data(const QModelIndex& index, int role) const {
	if (index.isValid()) {
        auto& file = m_files[index.row()];
        switch(role) {
            case IdRole:
                return file.id;
            case FileRole:
                return QFileInfo(file.path).fileName();
            case NameRole:
                return m_extras[file.id];
            case RuntimeRole: {
                QString runtime = QString("%1:%2")
                    .arg(file.runtime / 60)
                    .arg(file.runtime % 60, 2, 10, QChar('0'));
                return runtime;
            }
        }
    }
    return QVariant();
}

void Rd::Ui::Extras::AddExtras::handleFilesAdded(const QList<File>& files) {
    if (ModeHandler::instance()->mode() == ModeHandler::Extras) {
        beginResetModel();
        m_files.clear();
        m_extras.clear();
        for (auto& file: files) {
            m_files.append(file);
            QFileInfo f(file.path);
            m_extras.insert(file.id, f.completeBaseName().replace('.', ' '));
        }
        endResetModel();
        Q_EMIT requestOpenDialog();
    }
}
