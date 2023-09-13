#include "performancerecordview.h"

int Ruitie::PerformanceRecordView::rowCount(const QModelIndex &) const {
    return 15;
}

int Ruitie::PerformanceRecordView::columnCount(const QModelIndex &) const {
    return 6;
}

QVariant Ruitie::PerformanceRecordView::data(const QModelIndex &index, int role) const {
    if (index.row() == 0) {
        static QVector<QString> temp = {"测试项目", "灵敏度余量", "分辨力", "水平线性误差", "垂直线性误差", "动态范围"};
        return temp[index.column()];
    } else if (index.row() == 1) {
        if (index.column() == 0) {
            return "探头型号";
        }
        return "2.5M, 直探头";
    } else if (index.row() == 2) {
        if (index.column() == 0) {
            return "探头编号";
        }
        if (performanceRecordPtr) {
            return performanceRecordPtr->probeSerial[index.column() - 1];
        }
        return QVariant();
    } else if (index.row() == 3) {
        static QVector<QString> temp = {"试块型号", "CS-1-5", "CSK-1A", "CSK-1A", "CS-1-5", "CS-1-5"};
        return temp[index.column()];
    } else if (index.row() == 4) {
        static QVector<QString> temp = {"标准值", "≥42dB", "≥26dB", "≤2%", "≤6%", "≥26dB"};
        return temp[index.column()];
    } else {
        if (index.column() == 0) {
            static QVector<QString> temp = {"A1", "A2", "A3", "A4", "B1", "C1", "D1", "D2", "D3", "D4"};
            return temp[index.row() - 5];
        } else {
            if (performanceRecordPtr) {
                return performanceRecordPtr->performance[index.row() - 5][index.column() - 1];
            }
            return QVariant();
        }
    }
}

namespace Ruitie {
    QuarterlyRecordMsg *PerformanceRecordView::getPerformanceRecordPtr() const {
        return performanceRecordPtr;
    }

    void PerformanceRecordView::setPerformanceRecordPtr(QuarterlyRecordMsg *newPerformanceRecordPtr) {
        if (performanceRecordPtr == newPerformanceRecordPtr)
            return;
        performanceRecordPtr = newPerformanceRecordPtr;
        emit performanceRecordPtrChanged();
    }

} // namespace Ruitie
