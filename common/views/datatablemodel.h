#pragma once

#include "xlsxdocument.h"
#include <QAbstractTableModel>
#include <QtQml>
#include <memory>

using std::shared_ptr;

class DataTableModel : public QAbstractTableModel {
    Q_OBJECT
    QML_ELEMENT
public:
    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    QString          sExcelName() const;
    void             setSExcelName(const QString &newSExcelName);
    Q_INVOKABLE void loadExcel(void);
    QSize            span(const QModelIndex &index) const override {
        qDebug() << "call span";
        if (index.column() == 1 && index.row() == 1) {
            return QSize(6, 1);
        }
        return {1, 1};
    }
signals:
    void sExcelNameChanged();

private:
    std::shared_ptr<QXlsx::Document> m_pExcelDoc  = nullptr;
    QString                          m_sExcelName = "";
    Q_PROPERTY(QString sExcelName READ sExcelName WRITE setSExcelName NOTIFY sExcelNameChanged)
};
