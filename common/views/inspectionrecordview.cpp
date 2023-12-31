#include "inspectionrecordview.h"

int Ruitie::InspectionRecordView::rowCount(const QModelIndex &) const {
    return datNum + 1;
}

int Ruitie::InspectionRecordView::columnCount(const QModelIndex &) const {
    return 11;
}

QVariant Ruitie::InspectionRecordView::data(const QModelIndex &index, int role) const {
    if (index.row() == 0) {
        static QVector<QString> headName = {"序号", "车轮型号", "炉号", "车轮编号", "缺陷数量", "径向(mm)",
                                            "轴向(mm)", "波高(%)", "dB差(dB)", "探伤结果", "灵敏度(dB)"};
        return headName[index.column()];
    } else {
        int _i = index.row() - 1;
        switch (index.column()) {
            case 0: return index.row();
            case 1: return datptr[_i].wheelType;
            case 2: return datptr[_i].heatSerial;
            case 3: return datptr[_i].wheelSerial;
            case 4: return QString::number(datptr[_i].defectsNum, 'f', 1);
            case 5: return QString::number(datptr[_i].axial, 'f', 1);
            case 6: return QString::number(datptr[_i].radial, 'f', 1);
            case 7: return QString::number(datptr[_i].waveHeight, 'f', 1);
            case 8: return QString::number(datptr[_i].dBDiff, 'f', 1);
            case 9: return datptr[_i].result;
            case 10: return QString::number(datptr[_i].sensitivity, 'f', 1);
            default: break;
        }
    }
    return QVariant();
}

namespace Ruitie {

    std::shared_ptr<Ruitie::InspectionRecordModel[]> InspectionRecordView::getDatptr() const {
        return datptr;
    }

    void InspectionRecordView::setDatptr(const std::shared_ptr<Ruitie::InspectionRecordModel[]> &newDatptr) {
        if (datptr == newDatptr)
            return;
        datptr = newDatptr;
        emit datptrChanged();
    }

    int InspectionRecordView::getDatNum() const {
        return datNum;
    }

    void InspectionRecordView::setDatNum(int newDatNum) {
        if (datNum == newDatNum)
            return;
        datNum = newDatNum;
        emit datNumChanged();
    }

} // namespace Ruitie
