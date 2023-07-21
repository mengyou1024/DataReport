#pragma once
// 缺陷记录
#include "../ruitie.h"
#include <QAbstractTableModel>
#include <QObject>
#include <QQmlEngine>

namespace Ruitie {
    class DefectRecordView : public QAbstractTableModel {
        Q_OBJECT
        QML_ELEMENT
    public:
        std::shared_ptr<defectRecordModel[]> datptr = nullptr;

        int datNum = 0;

        int rowCount(const QModelIndex & = QModelIndex()) const override;

        int columnCount(const QModelIndex & = QModelIndex()) const override;

        QVariant data(const QModelIndex &index, int role) const override;

        QHash<int, QByteArray> roleNames() const override;

        std::shared_ptr<defectRecordModel[]> getDatptr() const;

        void setDatptr(const std::shared_ptr<defectRecordModel[]> &newDatptr);

        int  getDatNum() const;
        void setDatNum(int newDatNum);

        Q_INVOKABLE void resetModel(void) {
            beginResetModel();
            endResetModel();
        }

    signals:
        void datptrChanged();

        void datNumChanged();

    private:
        Q_PROPERTY(std::shared_ptr<defectRecordModel[]> datptr READ getDatptr WRITE setDatptr NOTIFY datptrChanged)
        Q_PROPERTY(int datNum READ getDatNum WRITE setDatNum NOTIFY datNumChanged)
    };
} // namespace Ruitie
