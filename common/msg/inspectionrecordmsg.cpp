#include "inspectionrecordmsg.h"

/**
 * @brief 保存文件 -- 特化(探伤记录表信息)
 * @param fileName
 * @param ptr
 * @return
 */
template <>
bool Ruitie::saveFile(QString &fileName, InspectionRecordMsg *ptr) {
    if (fileName.isEmpty() || !ptr) {
        qCritical(TAG_Ruitie) << "save InspectionRecordMsg in" << fileName << "failed";
        return false;
    }

    ExcelRender doc;

    doc.initColumnWidth(12, 11);
    doc.ExcelTableHead("车轮超声波探伤记录表");
    doc.ExcelNextRow();
    doc.ExcelFill<QString>(QString(), 9);
    doc.ExcelFill("日期:", ptr->detectDate);
    doc.ExcelNextRow();
    doc.ExcelFill("公司名", ptr->companyName, 10);
    doc.ExcelNextRow();
    doc.ExcelFill("设备名称", ptr->deviceName, 10);
    doc.ExcelNextRow();
    doc.ExcelFill("设备型号", ptr->deviceType, 4);
    doc.ExcelFill("设备编号", ptr->deviceSerial, 5);
    doc.ExcelNextRow();
    doc.ExcelFill("探伤标准", ptr->detectStandard, 10);
    doc.ExcelNextRow();
    doc.ExcelFill("探伤内容", ptr->detectContent, 10);
    doc.ExcelNextRow();
    doc.ExcelFill("探伤区域", ptr->detectRegion, 10);
    doc.ExcelNextRow();
    doc.ExcelFill("探头", ptr->probe, 4);
    doc.ExcelFill("声速", QString("%1 m/s").arg(ptr->soundSpeed), 5);
    doc.ExcelNextRow();
    for (auto it :
         {"序号", "车轮型号", "炉号", "车轮编号", "缺陷数目", "径向(mm)", "轴向(mm)", "波高(%)", "dB差(dB)", "探伤结果", "灵敏度(dB)"}) {
        doc.ExcelFill(it);
    }
    doc.ExcelNextRow();
    for (int i = 0; i < ptr->recordNum; i++) {
        doc.ExcelFill(i + 1);
        doc.ExcelFill(ptr->recordPtr[i].wheelType);
        doc.ExcelFill(ptr->recordPtr[i].heatSerial);
        doc.ExcelFill(ptr->recordPtr[i].wheelSerial);
        doc.ExcelFill(ptr->recordPtr[i].defectsNum);
        doc.ExcelFill(ptr->recordPtr[i].axial);
        doc.ExcelFill(ptr->recordPtr[i].radial);
        doc.ExcelFill(ptr->recordPtr[i].waveHeight);
        doc.ExcelFill(ptr->recordPtr[i].dBDiff);
        doc.ExcelFill(ptr->recordPtr[i].result);
        doc.ExcelFill(ptr->recordPtr[i].sensitivity);
        doc.ExcelNextRow();
    }

    if (doc.saveAs(fileName) == false) {
        qCritical(TAG_Ruitie) << "file" << fileName << "is being used";
        return false;
    }

    qInfo(TAG_Ruitie) << "save InspectionRecordMsg in" << fileName << "success";
    return true;
}

namespace Ruitie {

    QString InspectionRecordMsg::getDeviceName() const {
        return deviceName;
    }

    void InspectionRecordMsg::setDeviceName(const QString &newDeviceName) {
        if (deviceName == newDeviceName)
            return;
        deviceName = newDeviceName;
        emit deviceNameChanged();
    }

    QString InspectionRecordMsg::getDeviceType() const {
        return deviceType;
    }

    void InspectionRecordMsg::setDeviceType(const QString &newDeviceType) {
        if (deviceType == newDeviceType)
            return;
        deviceType = newDeviceType;
        emit deviceTypeChanged();
    }

    QString InspectionRecordMsg::getDeviceSerial() const {
        return deviceSerial;
    }

    void InspectionRecordMsg::setDeviceSerial(const QString &newDeviceSerial) {
        if (deviceSerial == newDeviceSerial)
            return;
        deviceSerial = newDeviceSerial;
        emit deviceSerialChanged();
    }

    QString InspectionRecordMsg::getDetectStandard() const {
        return detectStandard;
    }

    void InspectionRecordMsg::setDetectStandard(const QString &newDetectStandard) {
        if (detectStandard == newDetectStandard)
            return;
        detectStandard = newDetectStandard;
        emit detectStandardChanged();
    }

    QString InspectionRecordMsg::getDetectContent() const {
        return detectContent;
    }

    void InspectionRecordMsg::setDetectContent(const QString &newDetectContent) {
        if (detectContent == newDetectContent)
            return;
        detectContent = newDetectContent;
        emit detectContentChanged();
    }

    QString InspectionRecordMsg::getDetectRegion() const {
        return detectRegion;
    }

    void InspectionRecordMsg::setDetectRegion(const QString &newDetectRegion) {
        if (detectRegion == newDetectRegion)
            return;
        detectRegion = newDetectRegion;
        emit detectRegionChanged();
    }

    QString InspectionRecordMsg::getProbe() const {
        return probe;
    }

    void InspectionRecordMsg::setProbe(const QString &newProbe) {
        if (probe == newProbe)
            return;
        probe = newProbe;
        emit probeChanged();
    }

    qreal InspectionRecordMsg::getSoundSpeed() const {
        return soundSpeed;
    }

    void InspectionRecordMsg::setSoundSpeed(qreal newSoundSpeed) {
        if (qFuzzyCompare(soundSpeed, newSoundSpeed))
            return;
        soundSpeed = newSoundSpeed;
        emit soundSpeedChanged();
    }

    QString InspectionRecordMsg::getDetectDate() const {
        return detectDate;
    }

    void InspectionRecordMsg::setDetectDate(const QString &newDetectDate) {
        if (detectDate == newDetectDate)
            return;
        detectDate = newDetectDate;
        emit detectDateChanged();
    }

    shared_ptr<InspectionRecordModel[]> InspectionRecordMsg::getRecordPtr() const {
        return recordPtr;
    }

    void InspectionRecordMsg::setRecordPtr(const shared_ptr<InspectionRecordModel[]> &newRecordPtr) {
        if (recordPtr == newRecordPtr)
            return;
        recordPtr = newRecordPtr;
        emit recordPtrChanged();
    }

    int InspectionRecordMsg::getRecordNum() const {
        return recordNum;
    }

    void InspectionRecordMsg::setRecordNum(int newRecordNum) {
        if (recordNum == newRecordNum)
            return;
        recordNum = newRecordNum;
        emit recordNumChanged();
    }

    QString InspectionRecordMsg::getCompanyName() const {
        return companyName;
    }

    void InspectionRecordMsg::setCompanyName(const QString &newCompanyName) {
        if (companyName == newCompanyName)
            return;
        companyName = newCompanyName;
        emit companyNameChanged();
    }
} // namespace Ruitie
