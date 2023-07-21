#pragma once

#include "../ruitie.h"
#include "../ruitiedefine.h"

namespace Ruitie {
    /**
     * @brief 探伤记录表信息
     */
    class InspectionRecordMsg : public QObject {
        Q_OBJECT
    public:
        InspectionRecordMsg(QObject *parent = nullptr) {}
        QString companyName    = "";                             ///< 单位名称
        QString detectDate     = "";                             ///< 探伤日期
        QString deviceName     = "";                             ///< 设备名称
        QString deviceType     = "";                             ///< 设备型号
        QString deviceSerial   = "";                             ///< 设备编号
        QString detectStandard = "";                             ///< 探伤标准
        QString detectContent  = "";                             ///< 探伤内容
        QString detectRegion   = "";                             ///< 探伤区域
        QString probe          = "";                             ///< 探头
        qreal   soundSpeed     = 0;                              ///< 声速

        int                                 recordNum = 0;       ///< 记录数
        shared_ptr<InspectionRecordModel[]> recordPtr = nullptr; ///< 探伤记录指针

        QString getCompanyName() const;
        void    setCompanyName(const QString &newCompanyName);
        QString getDeviceName() const;
        void    setDeviceName(const QString &newDeviceName);
        QString getDeviceType() const;
        void    setDeviceType(const QString &newDeviceType);
        QString getDeviceSerial() const;
        void    setDeviceSerial(const QString &newDeviceSerial);
        QString getDetectStandard() const;
        void    setDetectStandard(const QString &newDetectStandard);
        QString getDetectContent() const;
        void    setDetectContent(const QString &newDetectContent);
        QString getDetectRegion() const;
        void    setDetectRegion(const QString &newDetectRegion);
        QString getProbe() const;
        void    setProbe(const QString &newProbe);
        qreal   getSoundSpeed() const;
        void    setSoundSpeed(qreal newSoundSpeed);

        Q_INVOKABLE bool saveFile(QString fileName) {
            return Ruitie::saveFile(fileName, this);
        }
        Q_INVOKABLE bool loadFile(QString fileName) {
#define READ_DATA(P)                                        \
    do {                                                    \
        file.read(reinterpret_cast<char *>(&P), sizeof(P)); \
    } while (0)

            QFile file(fileName);
            if (!file.exists()) {
                qCritical(TAG_Ruitie) << fileName << "is no exists";
                return false;
            }
            file.open(QFile::ReadOnly);
            if (!file.isOpen()) {
                qCritical(TAG_Ruitie) << fileName << "open failed";
                return false;
            }
            SYS_PARA      sysPara;
            CH_PARA       chPara;
            PLC_SCAN_PAPA plcScanPara;
            WHEEL_PAPA    wheelPara;
            DB_USER_DATA  dbUserData;
            DB_SCAN_DATA  dbScanData;
            READ_DATA(sysPara);
            READ_DATA(chPara);
            READ_DATA(plcScanPara);
            READ_DATA(wheelPara);
            READ_DATA(dbUserData);
            READ_DATA(dbScanData);
            DB_DEFECT_DATA dbDefectData[dbScanData.nTotalDefectNum];
            for (int i = 0; i < dbScanData.nTotalDefectNum; i++) {
                READ_DATA(dbDefectData[i]);
            }
            DB_SCAN_DATA nDbScanData[dbScanData.lTotalScanSize];
            for (int i = 0; i < dbScanData.lTotalScanSize; i++) {
                READ_DATA(nDbScanData[i]);
            }
            // 解析数据
            setCompanyName(QString(sysPara.szUseOrg));
            setDetectDate(QString(QDate::fromJulianDay(dbScanData.m_dtTime).toString("yyyy-MM-dd"))); // TODO: valid type
            setDeviceName(QString("TODO:deviceName"));                                                // TODO: devicename
            setDeviceType(QString(sysPara.szMadeModal));
            setDeviceSerial(QString(sysPara.szMadeSerial));
            setDetectStandard(QString("TODO:detectStandard")); // TODO: 探伤标准
            setDetectContent(QString("TODO: detectContent"));  // TODO: 探伤内容
            setDetectRegion(QString(chPara.szDetectionWheelArea));
            setProbe(QString(chPara.szProbeType));
            setSoundSpeed(chPara.nSpeed2); // TODO: valid type crash
            setRecordNum(dbScanData.nTotalDefectNum);

            setRecordPtr(shared_ptr<InspectionRecordModel[]>(new InspectionRecordModel[recordNum]));

            for (int i = 0; i < recordNum; i++) {
                recordPtr[i].wheelType   = QString("TODO: wheelType");            // TODO: wheelType
                recordPtr[i].heatSerial  = QString("TODO: heatSerial");           // TODO: heatSerial
                recordPtr[i].wheelSerial = QString(nDbScanData[i].szWheelNumber); // TODO: wheelSerial
                recordPtr[i].defectsNum  = nDbScanData[i].nTotalDefectNum;
                recordPtr[i].radial      = dbDefectData[i].nRadialDistance;
                recordPtr[i].axial       = dbDefectData[i].nAxialDepth;
                recordPtr[i].waveHeight  = dbDefectData[i].nWaveHeight;
                recordPtr[i].dBDiff      = dbDefectData[i].nDBOffset;
                recordPtr[i].result      = QString("TODO: result"); // TODO: result
                recordPtr[i].sensitivity = dbDefectData[i].nSensitivity;
            }

#undef READ_DATA

            file.close();
            return true;
        }

        QString getDetectDate() const;
        void    setDetectDate(const QString &newDetectDate);

        shared_ptr<InspectionRecordModel[]> getRecordPtr() const;

        void setRecordPtr(const shared_ptr<InspectionRecordModel[]> &newRecordPtr);

        int  getRecordNum() const;
        void setRecordNum(int newRecordNum);

    signals:
        void companyNameChanged();
        void detectDateChanged();
        void deviceNameChanged();
        void deviceTypeChanged();
        void deviceSerialChanged();
        void detectStandardChanged();
        void detectContentChanged();
        void detectRegionChanged();
        void probeChanged();
        void soundSpeedChanged();
        void recordPtrChanged();
        void recordNumChanged();

    private:
        Q_PROPERTY(QString companyName READ getCompanyName WRITE setCompanyName NOTIFY companyNameChanged)
        Q_PROPERTY(QString deviceName READ getDeviceName WRITE setDeviceName NOTIFY deviceNameChanged)
        Q_PROPERTY(QString deviceType READ getDeviceType WRITE setDeviceType NOTIFY deviceTypeChanged)
        Q_PROPERTY(QString deviceSerial READ getDeviceSerial WRITE setDeviceSerial NOTIFY deviceSerialChanged)
        Q_PROPERTY(QString detectStandard READ getDetectStandard WRITE setDetectStandard NOTIFY detectStandardChanged)
        Q_PROPERTY(QString detectContent READ getDetectContent WRITE setDetectContent NOTIFY detectContentChanged)
        Q_PROPERTY(QString detectRegion READ getDetectRegion WRITE setDetectRegion NOTIFY detectRegionChanged)
        Q_PROPERTY(QString probe READ getProbe WRITE setProbe NOTIFY probeChanged)
        Q_PROPERTY(qreal soundSpeed READ getSoundSpeed WRITE setSoundSpeed NOTIFY soundSpeedChanged)
        Q_PROPERTY(QString detectDate READ getDetectDate WRITE setDetectDate NOTIFY detectDateChanged)
        Q_PROPERTY(shared_ptr<InspectionRecordModel[]> recordPtr READ getRecordPtr WRITE setRecordPtr NOTIFY recordPtrChanged)
        Q_PROPERTY(int recordNum READ getRecordNum WRITE setRecordNum NOTIFY recordNumChanged)
    };

} // namespace Ruitie
