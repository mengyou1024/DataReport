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
        using SP_InsModel = shared_ptr<InspectionRecordModel[]>;
        InspectionRecordMsg(QObject *parent = nullptr) {}
        QString companyName    = "";     ///< 单位名称
        QString detectDate     = "";     ///< 探伤日期
        QString deviceName     = "";     ///< 设备名称
        QString deviceType     = "";     ///< 设备型号
        QString deviceSerial   = "";     ///< 设备编号
        QString detectStandard = "";     ///< 探伤标准
        QString detectContent  = "";     ///< 探伤内容
        QString detectRegion   = "";     ///< 探伤区域
        QString probe          = "";     ///< 探头
        qreal   soundSpeed     = 0;      ///< 声速

        int         recordNum = 0;       ///< 记录数
        SP_InsModel recordPtr = nullptr; ///< 探伤记录指针

        QString     getCompanyName() const;
        void        setCompanyName(const QString &newCompanyName);
        QString     getDeviceName() const;
        void        setDeviceName(const QString &newDeviceName);
        QString     getDeviceType() const;
        void        setDeviceType(const QString &newDeviceType);
        QString     getDeviceSerial() const;
        void        setDeviceSerial(const QString &newDeviceSerial);
        QString     getDetectStandard() const;
        void        setDetectStandard(const QString &newDetectStandard);
        QString     getDetectContent() const;
        void        setDetectContent(const QString &newDetectContent);
        QString     getDetectRegion() const;
        void        setDetectRegion(const QString &newDetectRegion);
        QString     getProbe() const;
        void        setProbe(const QString &newProbe);
        qreal       getSoundSpeed() const;
        void        setSoundSpeed(qreal newSoundSpeed);
        QString     getDetectDate() const;
        void        setDetectDate(const QString &newDetectDate);
        SP_InsModel getRecordPtr() const;
        void        setRecordPtr(const SP_InsModel &newRecordPtr);
        int         getRecordNum() const;
        void        setRecordNum(int newRecordNum);

        Q_INVOKABLE bool saveFile(QString fileName) {
            return Ruitie::saveFile(fileName, this);
        }
        Q_INVOKABLE bool loadFile(QString fileName) {
            auto recDataPtr = DataLoader::getRecData(fileName);
            if (recDataPtr == nullptr) {
                return false;
            }

            // 解析数据
            setCompanyName(QString::fromWCharArray(recDataPtr->wheelParam.szDetectionFact));
            setDetectDate(QDateTime::currentDateTime().toString("yyyy-M-d")); // TODO: 时间保存位置
            setDeviceName(QString::fromWCharArray(recDataPtr->wheelParam.szDeviceName));
            setDeviceType(QString::fromWCharArray(L""));                      // TODO: 设备型号保存位置
            setDeviceSerial(QString::fromWCharArray(L""));                    // TODO: 设备编号保存位置
            setDetectStandard(QString::fromWCharArray(recDataPtr->wheelParam.szDetectionStd));
            setDetectContent(QString::fromWCharArray(recDataPtr->wheelParam.szDetectionContent));
            setDetectRegion(QString::fromWCharArray(recDataPtr->wheelParam.szDetectionArea));
            setProbe(QString::fromWCharArray(L""));                 // TODO: 探头
            setSoundSpeed(recDataPtr->paramChannel[0].m_iVelocity); // TODO: valid type crash // TODO: 声速

            // TODO: 读取探伤数据
            setRecordNum(0);

            // setRecordPtr(shared_ptr<InspectionRecordModel[]>(new InspectionRecordModel[recordNum]));

            // for (int i = 0; i < recordNum; i++) {
            //     recordPtr[i].wheelType   = QString("TODO: wheelType");                            // TODO: wheelType
            //     recordPtr[i].heatSerial  = QString("TODO: heatSerial");                           // TODO: heatSerial
            //     recordPtr[i].wheelSerial = QString::fromStdWString(nDbScanData[i].szWheelNumber); // TODO: wheelSerial
            //     recordPtr[i].defectsNum  = nDbScanData[i].nTotalDefectNum;
            //     recordPtr[i].radial      = dbDefectData[i].nRadialDistance;
            //     recordPtr[i].axial       = dbDefectData[i].nAxialDepth;
            //     recordPtr[i].waveHeight  = dbDefectData[i].nWaveHeight;
            //     recordPtr[i].dBDiff      = dbDefectData[i].nDBOffset;
            //     recordPtr[i].result      = QString("TODO: result"); // TODO: result
            //     recordPtr[i].sensitivity = dbDefectData[i].nSensitivity;
            // }

            return true;
        }

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
        Q_PROPERTY(SP_InsModel recordPtr READ getRecordPtr WRITE setRecordPtr NOTIFY recordPtrChanged)
        Q_PROPERTY(int recordNum READ getRecordNum WRITE setRecordNum NOTIFY recordNumChanged)
    };

} // namespace Ruitie
