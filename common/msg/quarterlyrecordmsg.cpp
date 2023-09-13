#include "quarterlyrecordmsg.h"

/**
 * @brief 保存文件 -- 特化(季度性能检查表信息)
 * @param fileName
 * @param ptr
 * @return
 */
template <>
bool Ruitie::saveFile(QString &fileName, QuarterlyRecordMsg *ptr) {
    if (fileName.isEmpty() || !ptr) {
        qCritical(TAG_Ruitie) << "save QuarterlyRecordMsg in" << fileName << "failed";
        return false;
    }

    ExcelRender doc;

    doc.initColumnWidth({11.25, 3.0, 12.63, 13.13, 16.50, 15.00, 14.38});
    doc.ExcelTableHead("车轮超声波自动探伤机季度性能检查表");
    doc.ExcelNextRow();
    doc.ExcelFill<QString>(QString(), 5);
    doc.ExcelFill("打印日期:", ptr->printDate);
    doc.ExcelNextRow();
    doc.ExcelFill("单位名称", ptr->companyName, 6);
    doc.ExcelNextRow();
    doc.ExcelFill("仪器型号", ptr->instrumentType, 6);
    doc.ExcelNextRow();
    doc.ExcelFill("仪器编号", ptr->instrumentSerial, 6);
    doc.ExcelNextRow();
    doc.ExcelFill("制造单位", ptr->manufactureCompany, 6);
    doc.ExcelNextRow();
    doc.ExcelFill("制造日期:", ptr->manufactureDate, 6);
    doc.ExcelNextRow();
    doc.ExcelFill("测试项目", 2);
    for (auto it : {"灵敏度余量", "分辨力", "水平线性误差", "垂直线性误差", "动态范围"}) {
        doc.ExcelFill(it);
    }
    doc.ExcelNextRow();
    doc.ExcelFill("探头编号", 2);
    for (int i = 0; i < 5; i++) {
        doc.ExcelFill(ptr->probeSerial[i]);
    }
    doc.ExcelNextRow();
    doc.ExcelFill("试块型号", 2);
    for (auto it : {"CS-1-5", "CSK-1A", "CSK-1A", "CS-1-5", "CS-1-5"}) {
        doc.ExcelFill(it);
    }
    doc.ExcelNextRow();
    doc.ExcelFill("标准值", 2);
    for (auto it : {"≥46dB", "≥26dB", "≤0.5%", "≤5%", "≥26dB"}) {
        doc.ExcelFill(it);
    }
    doc.ExcelNextRow();
    int j = 0;
    for (auto it : {"A1", "A2", "A3", "A4", "B1", "C1", "D1", "D2", "D3", "D4"}) {
        doc.ExcelFill(it, 2);
        for (int i = 0; i < 5; i++) {
            doc.ExcelFill(ptr->performance[j][i]);
        }
        doc.ExcelNextRow();
        j++;
    }

    if (doc.saveAs(fileName) == false) {
        qCritical(TAG_Ruitie) << "file" << fileName << "is being used";
        return false;
    }

    qInfo(TAG_Ruitie) << "save QuarterlyRecordMsg in" << fileName << "success";
    return true;
}

namespace Ruitie {
    QString QuarterlyRecordMsg::getInstrumentType() const {
        return instrumentType;
    }

    void QuarterlyRecordMsg::setInstrumentType(const QString &newInstrumentType) {
        if (instrumentType == newInstrumentType)
            return;
        instrumentType = newInstrumentType;
        emit instrumentTypeChanged();
    }

    QString QuarterlyRecordMsg::getInstrumentSerial() const {
        return instrumentSerial;
    }

    void QuarterlyRecordMsg::setInstrumentSerial(const QString &newInstrumentSerial) {
        if (instrumentSerial == newInstrumentSerial)
            return;
        instrumentSerial = newInstrumentSerial;
        emit instrumentSerialChanged();
    }

    QString QuarterlyRecordMsg::getManufactureCompany() const {
        return manufactureCompany;
    }

    void QuarterlyRecordMsg::setManufactureCompany(const QString &newManufactureCompany) {
        if (manufactureCompany == newManufactureCompany)
            return;
        manufactureCompany = newManufactureCompany;
        emit manufactureCompanyChanged();
    }

    QString QuarterlyRecordMsg::getManufactureDate() const {
        return manufactureDate;
    }

    void QuarterlyRecordMsg::setManufactureDate(const QString &newManufactureDate) {
        if (manufactureDate == newManufactureDate)
            return;
        manufactureDate = newManufactureDate;
        emit manufactureDateChanged();
    }

    QString QuarterlyRecordMsg::getPrintDate() const {
        return printDate;
    }

    void QuarterlyRecordMsg::setPrintDate(const QString &newPrintDate) {
        if (printDate == newPrintDate)
            return;
        printDate = newPrintDate;
        emit printDateChanged();
    }

    QString QuarterlyRecordMsg::getCompanyName() const {
        return companyName;
    }

    void QuarterlyRecordMsg::setCompanyName(const QString &newCompanyName) {
        if (companyName == newCompanyName)
            return;
        companyName = newCompanyName;
        emit companyNameChanged();
    }
} // namespace Ruitie
