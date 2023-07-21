#include "datatablemodel.h"
#include <QDebug>

#include "xlsxdocument.h"
#include <memory>
using std::shared_ptr;

int DataTableModel::rowCount(const QModelIndex &) const {
    if (!m_pExcelDoc || !m_pExcelDoc->load()) {
        return 1;
    }
    return m_pExcelDoc->dimension().rowCount();
}

int DataTableModel::columnCount(const QModelIndex &) const {
    if (!m_pExcelDoc || !m_pExcelDoc->load()) {
        return 1;
    }
    return m_pExcelDoc->dimension().columnCount();
}

QVariant DataTableModel::data(const QModelIndex &index, int role) const {
    if (!m_pExcelDoc || !m_pExcelDoc->load()) {
        return QString("read:%1 error").arg(m_sExcelName);
    }
    switch (role) {
        case Qt::DisplayRole: {
            auto cell = m_pExcelDoc->cellAt(index.row() + 1, index.column() + 1);
            if (cell) {
                qDebug() << "row:" << index.row() << "column:" << index.column() << "value:" << cell->value();
                return cell->value();
            }
            break;
        }
        default: break;
    }

    return QVariant();
}

QHash<int, QByteArray> DataTableModel::roleNames() const {
    return {
        {Qt::DisplayRole, "display"}
    };
}

QString DataTableModel::sExcelName() const {
    return m_sExcelName;
}

void DataTableModel::setSExcelName(const QString &newSExcelName) {
    if (m_sExcelName == newSExcelName)
        return;
    m_sExcelName = newSExcelName;
    beginResetModel();
    loadExcel();
    endResetModel();
    emit sExcelNameChanged();
}

void DataTableModel::loadExcel() {
    if (!m_sExcelName.isEmpty()) {
        m_pExcelDoc = std::make_shared<QXlsx::Document>(m_sExcelName);
        if (!m_pExcelDoc->load()) {
            qWarning() << "read excel :" << m_sExcelName << "error";
        } else {
            qDebug() << "load Excel:" << m_sExcelName;
        }
    } else {
        m_pExcelDoc = nullptr;
        qWarning() << "m_sExcelName is empty";
    }
}
