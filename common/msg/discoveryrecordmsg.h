#pragma once

#include "../ruitie.h"
#include "../ruitiedefine.h"

namespace Ruitie {

    /**
     * @brief 探伤发现缺陷记录表信息
     */
    class DiscoveryRecordMsg : public QObject {
        Q_OBJECT
    public:
        DiscoveryRecordMsg(QObject *parent = nullptr) {}
        QString companyName = "";                                              ///< 单位名称
        QString detectDate  = "";                                              ///< 探伤日期
        qreal   workFreq    = 0;                                               ///< 工作频率
        QString probe       = "";                                              ///< 探头
        QString coupledMode = "";                                              ///< 耦合方式
        QString wheelType   = "";                                              ///< 车轮型号
        QString wheelSerial = "";                                              ///< 车轮编号
        QString heatSerial  = "";                                              ///< 炉号
        int     defectsNum  = 0;                                               ///< 缺陷数目

        shared_ptr<defectRecordModel[]> defectEchoRecord            = nullptr; ///< 缺陷回波记录
        shared_ptr<defectRecordModel[]> bottomWaveAttenuationRecord = nullptr; ///< 底波衰减记录

        QString result = "";                                                   ///< 探伤结论

        QString getCompanyName() const;
        void    setCompanyName(const QString &newCompanyName);
        qreal   getWorkFreq() const;
        void    setWorkFreq(qreal newWorkFreq);
        QString getProbe() const;
        void    setProbe(const QString &newProbe);
        QString getCoupledMode() const;
        void    setCoupledMode(const QString &newCoupledMode);
        QString getWheelType() const;
        void    setWheelType(const QString &newWheelType);
        QString getWheelSerial() const;
        void    setWheelSerial(const QString &newWheelSerial);
        QString getHeatSerial() const;
        void    setHeatSerial(const QString &newHeatSerial);

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
            setWorkFreq(chPara.nProbeFrequency);
            setProbe(chPara.szProbeType);
            setCoupledMode("水浸");
            setWheelType(QString(wheelPara.szWheelType));
            setHeatSerial(QString(wheelPara.szHeatNumber));
            setWheelSerial(QString(wheelPara.szWheelNumber));
            setDefectsNum(dbScanData.nTotalDefectNum);

            setDefectEchoRecord(shared_ptr<defectRecordModel[]>(new defectRecordModel[defectsNum]));
            setBottomWaveAttenuationRecord(shared_ptr<defectRecordModel[]>(new defectRecordModel[defectsNum]));
            for (int i = 0; i < defectsNum; i++) {
                defectEchoRecord[i].axial      = dbDefectData[i].nAxialDepth;
                defectEchoRecord[i].radial     = dbDefectData[i].nRadialDistance;
                defectEchoRecord[i].gain       = dbDefectData[i].nParam1; // TODO: 获取增益
                defectEchoRecord[i].waveHeight = dbDefectData[i].nWaveHeight;
                defectEchoRecord[i].dBDiff     = dbDefectData[i].nDBOffset;

                bottomWaveAttenuationRecord[i].axial      = dbDefectData[i].nAxialDepth;
                bottomWaveAttenuationRecord[i].radial     = dbDefectData[i].nRadialDistance;
                bottomWaveAttenuationRecord[i].gain       = dbDefectData[i].nParam1; // TODO: 获取增益
                bottomWaveAttenuationRecord[i].waveHeight = dbDefectData[i].nWaveHeight;
                bottomWaveAttenuationRecord[i].dBDiff     = dbDefectData[i].nDBOffset;
            }

#undef READ_DATA

            file.close();
            return true;
        }

        QString getDetectDate() const;
        void    setDetectDate(const QString &newDetectDate);

        shared_ptr<defectRecordModel[]> getDefectEchoRecord() const;

        void setDefectEchoRecord(const shared_ptr<defectRecordModel[]> &newDefectEchoRecord);

        shared_ptr<defectRecordModel[]> getBottomWaveAttenuationRecord() const;

        void setBottomWaveAttenuationRecord(const shared_ptr<defectRecordModel[]> &newBottomWaveAttenuationRecord);

        int  getDefectsNum() const;
        void setDefectsNum(int newDefectsNum);

    signals:
        void companyNameChanged();
        void detectDateChanged();
        void workFreqChanged();
        void probeChanged();
        void coupledModeChanged();
        void wheelTypeChanged();
        void wheelSerialChanged();
        void heatSerialChanged();

        void defectEchoRecordChanged();

        void bottomWaveAttenuationRecordChanged();

        void defectsNumChanged();

    private:
        Q_PROPERTY(QString companyName READ getCompanyName WRITE setCompanyName NOTIFY companyNameChanged)
        Q_PROPERTY(qreal workFreq READ getWorkFreq WRITE setWorkFreq NOTIFY workFreqChanged)
        Q_PROPERTY(QString probe READ getProbe WRITE setProbe NOTIFY probeChanged)
        Q_PROPERTY(QString coupledMode READ getCoupledMode WRITE setCoupledMode NOTIFY coupledModeChanged)
        Q_PROPERTY(QString wheelType READ getWheelType WRITE setWheelType NOTIFY wheelTypeChanged)
        Q_PROPERTY(QString wheelSerial READ getWheelSerial WRITE setWheelSerial NOTIFY wheelSerialChanged)
        Q_PROPERTY(QString heatSerial READ getHeatSerial WRITE setHeatSerial NOTIFY heatSerialChanged)
        Q_PROPERTY(QString detectDate READ getDetectDate WRITE setDetectDate NOTIFY detectDateChanged)
        Q_PROPERTY(shared_ptr<defectRecordModel[]> defectEchoRecord READ getDefectEchoRecord WRITE setDefectEchoRecord NOTIFY
                       defectEchoRecordChanged)
        Q_PROPERTY(shared_ptr<defectRecordModel[]> bottomWaveAttenuationRecord READ getBottomWaveAttenuationRecord WRITE
                       setBottomWaveAttenuationRecord NOTIFY bottomWaveAttenuationRecordChanged)
        Q_PROPERTY(int defectsNum READ getDefectsNum WRITE setDefectsNum NOTIFY defectsNumChanged)
    };

} // namespace Ruitie
