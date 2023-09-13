#include "discoveryrecordmsg.h"

/**
 * @brief 保存文件 -- 特化(探伤发现缺陷记录表信息)
 * @param fileName
 * @param ptr
 * @return
 */
template <>
bool Ruitie::saveFile(QString &fileName, DiscoveryRecordMsg *ptr) {
    if (fileName.isEmpty() || !ptr) {
        qCritical(TAG_Ruitie) << "save DiscoveryRecordMsg in" << fileName << "failed";
        return false;
    }
    ExcelRender doc;

    doc.initColumnWidth({8.38, 13.88, 11.0, 11.88, 18.88, 15.63});
    doc.ExcelTableHead("车轮超声波探伤发现记录表");
    doc.ExcelNextRow();
    doc.ExcelFill("单位名称:", ptr->companyName, 3);
    doc.ExcelFill("探伤日期:", ptr->detectDate);
    doc.ExcelNextRow();
    doc.ExcelFill("探伤条件", 2, 2);
    doc.ExcelFill("工作频率:", QString("%1MHz").arg(ptr->workFreq));
    doc.ExcelFill("探头:", ptr->probe);
    doc.ExcelNextRow(2);
    doc.ExcelFill("耦合方式:", ptr->coupledMode, 3);
    doc.ExcelNextRow();
    doc.ExcelFill("车轮信息:", 2, 2);
    doc.ExcelFill("车轮型号:", ptr->wheelType, 3);
    doc.ExcelNextRow(2);
    doc.ExcelFill("车轮编号:", ptr->wheelSerial);
    doc.ExcelFill("炉号:", ptr->heatSerial);
    doc.ExcelNextRow();
    doc.ExcelFill("缺陷数目:", QString("共 %1 处").arg(ptr->defectsNum), 4, 1, 2, 1);
    doc.ExcelNextRow();
    doc.ExcelFill("缺陷回波记录", 6);
    doc.ExcelNextRow();
    doc.ExcelFill("缺陷编号", 1, 2);
    doc.ExcelFill("缺陷位置(mm)", "缺陷波探伤", 3, 1, 2, 1);
    doc.ExcelNextRow(1, 28.5);
    for (auto it : {"轴向", "径向", "增益(dB)", "缺陷波高(%)", "dB差(dB)"}) {
        doc.ExcelFill(it);
    }
    doc.ExcelNextRow();
    for (int i = 0; i < ptr->defectsNum; i++) {
        doc.ExcelFill(i + 1);
        doc.ExcelFill(ptr->defectEchoRecord[i].axial);
        doc.ExcelFill(ptr->defectEchoRecord[i].radial);
        doc.ExcelFill(ptr->defectEchoRecord[i].gain);
        doc.ExcelFill(ptr->defectEchoRecord[i].waveHeight);
        doc.ExcelFill(ptr->defectEchoRecord[i].dBDiff);
        doc.ExcelNextRow();
    }
    doc.ExcelFill("底波衰减记录", 6);
    doc.ExcelNextRow();
    doc.ExcelFill("缺陷编号", 1, 2);
    doc.ExcelFill("缺陷位置(mm)", "缺陷波探伤", 3, 1, 2, 1);
    doc.ExcelNextRow(1, 28.5);
    for (auto it : {"轴向", "径向", "增益(dB)", "缺陷波高(%)", "dB差(dB)"}) {
        doc.ExcelFill(it);
    }

    for (int i = 0; i < ptr->defectsNum; i++) {
        doc.ExcelNextRow();
        doc.ExcelFill(i + 1);
        doc.ExcelFill(ptr->bottomWaveAttenuationRecord[i].axial);
        doc.ExcelFill(ptr->bottomWaveAttenuationRecord[i].radial);
        doc.ExcelFill(ptr->bottomWaveAttenuationRecord[i].gain);
        doc.ExcelFill(ptr->bottomWaveAttenuationRecord[i].waveHeight);
        doc.ExcelFill(ptr->bottomWaveAttenuationRecord[i].dBDiff);
    }
    doc.ExcelNextRow(0, 35.25);
    doc.ExcelFill("探伤结论", ptr->result, 4, 1, 2, 1);
    doc.ExcelNextRow(0, 31.50);
    doc.ExcelFill("签字栏", 2, 2);
    doc.ExcelFill("探伤工:", QString());
    doc.ExcelFill("探伤工长:", QString());
    doc.ExcelNextRow(2, 31.50);
    doc.ExcelFill("质检员:", QString());
    doc.ExcelFill("验收员:", QString());

    // 保存表
    if (doc.saveAs(fileName) == false) {
        qCritical(TAG_Ruitie) << "file" << fileName << "is being used";
        return false;
    }

    qInfo(TAG_Ruitie()) << "save DiscoveryRecordMsg in" << fileName << "success";
    return true;
}

namespace Ruitie {

    qreal DiscoveryRecordMsg::getWorkFreq() const {
        return workFreq;
    }

    void DiscoveryRecordMsg::setWorkFreq(qreal newWorkFreq) {
        if (qFuzzyCompare(workFreq, newWorkFreq))
            return;
        workFreq = newWorkFreq;
        emit workFreqChanged();
    }

    QString DiscoveryRecordMsg::getProbe() const {
        return probe;
    }

    void DiscoveryRecordMsg::setProbe(const QString &newProbe) {
        if (probe == newProbe)
            return;
        probe = newProbe;
        emit probeChanged();
    }

    QString DiscoveryRecordMsg::getCoupledMode() const {
        return coupledMode;
    }
    void DiscoveryRecordMsg::setCoupledMode(const QString &newCoupledMode) {
        if (coupledMode == newCoupledMode)
            return;
        coupledMode = newCoupledMode;
        emit coupledModeChanged();
    }

    QString DiscoveryRecordMsg::getWheelType() const {
        return wheelType;
    }

    void DiscoveryRecordMsg::setWheelType(const QString &newWheelType) {
        if (wheelType == newWheelType)
            return;
        wheelType = newWheelType;
        emit wheelTypeChanged();
    }

    QString DiscoveryRecordMsg::getWheelSerial() const {
        return wheelSerial;
    }

    void DiscoveryRecordMsg::setWheelSerial(const QString &newWheelSerial) {
        if (wheelSerial == newWheelSerial)
            return;
        wheelSerial = newWheelSerial;
        emit wheelSerialChanged();
    }

    QString DiscoveryRecordMsg::getHeatSerial() const {
        return heatSerial;
    }

    void DiscoveryRecordMsg::setHeatSerial(const QString &newHeatSerial) {
        if (heatSerial == newHeatSerial)
            return;
        heatSerial = newHeatSerial;
        emit heatSerialChanged();
    }

    QString DiscoveryRecordMsg::getDetectDate() const {
        return detectDate;
    }

    void DiscoveryRecordMsg::setDetectDate(const QString &newDetectDate) {
        if (detectDate == newDetectDate)
            return;
        detectDate = newDetectDate;
        emit detectDateChanged();
    }

    shared_ptr<defectRecordModel[]> DiscoveryRecordMsg::getDefectEchoRecord() const {
        return defectEchoRecord;
    }

    void DiscoveryRecordMsg::setDefectEchoRecord(const shared_ptr<defectRecordModel[]> &newDefectEchoRecord) {
        if (defectEchoRecord == newDefectEchoRecord)
            return;
        defectEchoRecord = newDefectEchoRecord;
        emit defectEchoRecordChanged();
    }

    shared_ptr<defectRecordModel[]> DiscoveryRecordMsg::getBottomWaveAttenuationRecord() const {
        return bottomWaveAttenuationRecord;
    }

    void DiscoveryRecordMsg::setBottomWaveAttenuationRecord(const shared_ptr<defectRecordModel[]> &newBottomWaveAttenuationRecord) {
        if (bottomWaveAttenuationRecord == newBottomWaveAttenuationRecord)
            return;
        bottomWaveAttenuationRecord = newBottomWaveAttenuationRecord;
        emit bottomWaveAttenuationRecordChanged();
    }

    int DiscoveryRecordMsg::getDefectsNum() const {
        return defectsNum;
    }

    void DiscoveryRecordMsg::setDefectsNum(int newDefectsNum) {
        if (defectsNum == newDefectsNum)
            return;
        defectsNum = newDefectsNum;
        emit defectsNumChanged();
    }

    QString DiscoveryRecordMsg::getCompanyName() const {
        return companyName;
    }

    void DiscoveryRecordMsg::setCompanyName(const QString &newCompanyName) {
        if (companyName == newCompanyName)
            return;
        companyName = newCompanyName;
        emit companyNameChanged();
    }
} // namespace Ruitie
