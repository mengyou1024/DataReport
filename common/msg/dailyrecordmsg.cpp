#include "dailyrecordmsg.h"

/**
 * @brief 保存文件 -- 特化(日常性能检查表信息)
 * @param fileName
 * @param ptr
 * @return
 */
template <>
bool Ruitie::saveFile(QString &fileName, DailyRecordMsg *ptr) {
    if (fileName.isEmpty() || !ptr) {
        qCritical(TAG_Ruitie) << "save DailyRecordMsg in" << fileName << "failed";
        return false;
    }

    ExcelRender doc;

    doc.initColumnWidth({8, 8, 8, 10, 16, 20});
    doc.ExcelTableHead("车轮超声波自动探伤机日常性能检查记录表");
    doc.ExcelNextRow();
    doc.ExcelFill("单位名称:", ptr->companyName, 3, 1, 2, 1);
    doc.ExcelFill(QString("探伤日期:%1").arg(ptr->detectDate));
    doc.ExcelNextRow();
    doc.ExcelFill("探伤设备:", ptr->detectDeive, 3, 1, 2, 1);
    doc.ExcelFill(QString("工作频率: %1 MHz").arg(ptr->workFreq));
    doc.ExcelNextRow();
    doc.ExcelFill("探头:", ptr->probe, 2, 1, 2, 1);
    doc.ExcelFill("耦合方式:", ptr->coupledMode);
    doc.ExcelNextRow();
    doc.ExcelFill("车轮型号:", ptr->wheelType, 2, 1, 2, 1);
    doc.ExcelFill("炉号:", ptr->heatSerial);
    doc.ExcelNextRow();
    doc.ExcelFill("车轮编号:", ptr->wheelSerial, 4, 1, 2, 1);
    doc.ExcelNextRow();
    doc.ExcelFill("综合检查项目", 6);
    doc.ExcelNextRow();
    doc.ExcelFill("缺陷数目:", QString("共 %1 处").arg(ptr->defectsNum), 4, 1, 2, 1);
    doc.ExcelNextRow();
    doc.ExcelFill("缺陷网表记录", 6);
    doc.ExcelNextRow();
    doc.ExcelFill("缺陷编号", 1, 2);
    doc.ExcelFill("缺陷位置(mm)", 2);
    doc.ExcelFill("缺陷波探伤", 3);
    doc.ExcelNextRow(1);
    for (auto it : {"轴向", "径向", "增益(dB)", "缺陷波高(%)", "dB差(dB)"}) {
        doc.ExcelFill(it);
    }
    doc.ExcelNextRow();

    for (int i = 0; i < ptr->defectsNum; i++) {
        doc.ExcelFill(i + 1);
        doc.ExcelFill(ptr->defectsPtr[i].axial);
        doc.ExcelFill(ptr->defectsPtr[i].radial);
        doc.ExcelFill(ptr->defectsPtr[i].gain);
        doc.ExcelFill(ptr->defectsPtr[i].waveHeight / (qreal)(2.55));
        doc.ExcelFill(ptr->defectsPtr[i].dBDiff);
        doc.ExcelNextRow();
    }

    if (doc.saveAs(fileName) == false) {
        qCritical(TAG_Ruitie) << "file" << fileName << "is being used";
        return false;
    }
    qInfo(TAG_Ruitie) << "save DailyRecordMsg in" << fileName << "success";
    return true;
}

namespace Ruitie {

    QString DailyRecordMsg::getDetectDeive() const {
        return detectDeive;
    }

    void DailyRecordMsg::setDetectDeive(const QString &newDetectDeive) {
        if (detectDeive == newDetectDeive)
            return;
        detectDeive = newDetectDeive;
        emit detectDeiveChanged();
    }

    qreal DailyRecordMsg::getWorkFreq() const {
        return workFreq;
    }

    void DailyRecordMsg::setWorkFreq(qreal newWorkFreq) {
        if (qFuzzyCompare(workFreq, newWorkFreq))
            return;
        workFreq = newWorkFreq;
        emit workFreqChanged();
    }

    QString DailyRecordMsg::getProbe() const {
        return probe;
    }

    void DailyRecordMsg::setProbe(const QString &newProbe) {
        if (probe == newProbe)
            return;
        probe = newProbe;
        emit probeChanged();
    }

    QString DailyRecordMsg::getCoupledMode() const {
        return coupledMode;
    }

    void DailyRecordMsg::setCoupledMode(const QString &newCoupledMode) {
        if (coupledMode == newCoupledMode)
            return;
        coupledMode = newCoupledMode;
        emit coupledModeChanged();
    }

    QString DailyRecordMsg::getWheelType() const {
        return wheelType;
    }

    void DailyRecordMsg::setWheelType(const QString &newWheelType) {
        if (wheelType == newWheelType)
            return;
        wheelType = newWheelType;
        emit wheelTypeChanged();
    }

    QString DailyRecordMsg::getHeatSerial() const {
        return heatSerial;
    }

    void DailyRecordMsg::setHeatSerial(const QString &newHeatSerial) {
        if (heatSerial == newHeatSerial)
            return;
        heatSerial = newHeatSerial;
        emit heatSerialChanged();
    }
    QString DailyRecordMsg::getWheelSerial() const {
        return wheelSerial;
    }

    void DailyRecordMsg::setWheelSerial(const QString &newWheelSerial) {
        if (wheelSerial == newWheelSerial)
            return;
        wheelSerial = newWheelSerial;
        emit wheelSerialChanged();
    }

    QString DailyRecordMsg::getDetectDate() const {
        return detectDate;
    }

    void DailyRecordMsg::setDetectDate(const QString &newDetectDate) {
        if (detectDate == newDetectDate)
            return;
        detectDate = newDetectDate;
        emit detectDateChanged();
    }

    shared_ptr<defectRecordModel[]> DailyRecordMsg::getDefectsPtr() const {
        return defectsPtr;
    }

    void DailyRecordMsg::setDefectsPtr(const shared_ptr<defectRecordModel[]> &newDefectsPtr) {
        if (defectsPtr == newDefectsPtr)
            return;
        defectsPtr = newDefectsPtr;
        emit defectsPtrChanged();
    }

    int DailyRecordMsg::getDefectsNum() const {
        return defectsNum;
    }

    void DailyRecordMsg::setDefectsNum(int newDefectsNum) {
        if (defectsNum == newDefectsNum)
            return;
        defectsNum = newDefectsNum;
        emit defectsNumChanged();
    }

    QString DailyRecordMsg::getCompanyName() const {
        return companyName;
    }

    void DailyRecordMsg::setCompanyName(const QString &newCompanyName) {
        if (companyName == newCompanyName)
            return;
        companyName = newCompanyName;
        emit companyNameChanged();
    }

} // namespace Ruitie
