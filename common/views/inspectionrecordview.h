#pragma once
// 探伤记录
#include "../ruitie.h"
#include <QAbstractTableModel>
#include <QObject>
#include <QQmlEngine>

namespace Ruitie {
    class InspectionRecordView : public QAbstractTableModel {
        Q_OBJECT
        QML_ELEMENT
    public:
        std::shared_ptr<Ruitie::InspectionRecordModel[]> datptr = nullptr;

        int datNum = 0;

        int rowCount(const QModelIndex & = QModelIndex()) const override;

        int columnCount(const QModelIndex & = QModelIndex()) const override;

        QVariant data(const QModelIndex &index, int role) const override;

        QHash<int, QByteArray> roleNames() const override {
            return {
                {Qt::DisplayRole, "display"}
            };
        }

        Q_INVOKABLE void resetModel(void) {
            beginResetModel();
            endResetModel();
        }

        int  getDatNum() const;
        void setDatNum(int newDatNum);

        std::shared_ptr<Ruitie::InspectionRecordModel[]> getDatptr() const;

        void setDatptr(const std::shared_ptr<Ruitie::InspectionRecordModel[]> &newDatptr);

    signals:
        void datNumChanged();
        void datptrChanged();

    private:
        Q_PROPERTY(int datNum READ getDatNum WRITE setDatNum NOTIFY datNumChanged)
        Q_PROPERTY(std::shared_ptr<Ruitie::InspectionRecordModel[]> datptr READ getDatptr WRITE setDatptr NOTIFY datptrChanged)
    };
} // namespace Ruitie
