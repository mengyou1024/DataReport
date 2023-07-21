#include "defectrecordview.h"

namespace Ruitie {
    int DefectRecordView::getDatNum() const {
        return datNum;
    }

    void DefectRecordView::setDatNum(int newDatNum) {
        if (datNum == newDatNum)
            return;
        datNum = newDatNum;
        emit datNumChanged();
    }

    int DefectRecordView::rowCount(const QModelIndex &) const {
        return datNum + 1;
    }

    int DefectRecordView::columnCount(const QModelIndex &) const {
        return 6;
    }

    QVariant DefectRecordView::data(const QModelIndex &index, int role) const {
        if (index.row() == 0) {
            static QList<QString> headName = {"缺陷编号", "轴向", "径向", "增益(dB)", "缺陷波高(%)", "dB差(dB)"};
            return headName[index.column()];
        } else {
            switch (index.column()) {
                case 0: return index.row() - 1;
                case 1: return datptr[index.row() - 1].axial;
                case 2: return datptr[index.row() - 1].radial;
                case 3: return datptr[index.row() - 1].gain;
                case 4: return datptr[index.row() - 1].waveHeight;
                case 5: return datptr[index.row() - 1].dBDiff;
                default: break;
            }
        }
        return QVariant();
    }

    QHash<int, QByteArray> DefectRecordView::roleNames() const {
        return {
            {Qt::DisplayRole, "display"}
        };
    }

    std::shared_ptr<defectRecordModel[]> DefectRecordView::getDatptr() const {
        return datptr;
    }

    void DefectRecordView::setDatptr(const std::shared_ptr<defectRecordModel[]> &newDatptr) {
        if (datptr == newDatptr)
            return;
        datptr = newDatptr;
        beginResetModel();
        endResetModel();

        emit datptrChanged();
    }

} // namespace Ruitie
