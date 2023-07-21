#pragma once
// 性能记录
#include "../msg/quarterlyrecordmsg.h"
#include <QAbstractTableModel>
#include <QObject>
#include <QQmlEngine>

namespace Ruitie {
    class PerformanceRecordView : public QAbstractTableModel {
        Q_OBJECT
        QML_ELEMENT
    public:
        QuarterlyRecordMsg *performanceRecordPtr = nullptr;

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
        QuarterlyRecordMsg *getPerformanceRecordPtr() const;
        void                setPerformanceRecordPtr(QuarterlyRecordMsg *newPerformanceRecordPtr);
    signals:
        void performanceRecordPtrChanged();

    private:
        Q_PROPERTY(QuarterlyRecordMsg *performanceRecordPtr READ getPerformanceRecordPtr WRITE setPerformanceRecordPtr NOTIFY
                       performanceRecordPtrChanged)
    };

} // namespace Ruitie
