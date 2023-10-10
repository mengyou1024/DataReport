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
        using SP_DefModel = shared_ptr<defectRecordModel[]>;
        DiscoveryRecordMsg(QObject *parent = nullptr) {}
        QString     companyName                 = "";      ///< 单位名称
        QString     detectDate                  = "";      ///< 探伤日期
        qreal       workFreq                    = 0;       ///< 工作频率
        QString     probe                       = "";      ///< 探头
        QString     coupledMode                 = "";      ///< 耦合方式
        QString     wheelType                   = "";      ///< 车轮型号
        QString     wheelSerial                 = "";      ///< 车轮编号
        QString     heatSerial                  = "";      ///< 炉号
        int         defectsNum                  = 0;       ///< 缺陷数目
        SP_DefModel defectEchoRecord            = nullptr; ///< 缺陷回波记录
        SP_DefModel bottomWaveAttenuationRecord = nullptr; ///< 底波衰减记录

        QString result = "";                               ///< 探伤结论

        QString     getCompanyName() const;
        void        setCompanyName(const QString &newCompanyName);
        qreal       getWorkFreq() const;
        void        setWorkFreq(qreal newWorkFreq);
        QString     getProbe() const;
        void        setProbe(const QString &newProbe);
        QString     getCoupledMode() const;
        void        setCoupledMode(const QString &newCoupledMode);
        QString     getWheelType() const;
        void        setWheelType(const QString &newWheelType);
        QString     getWheelSerial() const;
        void        setWheelSerial(const QString &newWheelSerial);
        QString     getHeatSerial() const;
        void        setHeatSerial(const QString &newHeatSerial);
        QString     getDetectDate() const;
        void        setDetectDate(const QString &newDetectDate);
        SP_DefModel getDefectEchoRecord() const;
        void        setDefectEchoRecord(const shared_ptr<defectRecordModel[]> &newDefectEchoRecord);
        SP_DefModel getBottomWaveAttenuationRecord() const;
        void        setBottomWaveAttenuationRecord(const SP_DefModel &newBottomWaveAttenuationRecord);
        int         getDefectsNum() const;
        void        setDefectsNum(int newDefectsNum);

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
            setWorkFreq(0);                                                   // TODO: 探头频率保存位置
            setProbe(QString::fromWCharArray(L""));                           // TODO: 探头保存位置
            setCoupledMode(QString::fromWCharArray(L"水浸"));                 // TODO: 耦合方式
            setWheelType(QString::fromWCharArray(recDataPtr->wheelParam.szWheelType));
            setHeatSerial(QString::fromWCharArray(recDataPtr->wheelParam.szHeatNumber));
            setWheelSerial(QString::fromWCharArray(recDataPtr->wheelParam.szWheelNumber));
            setDefectsNum(0);

            // 填充缺陷数据
            for (uint32_t i = 0; i < HD_CHANNEL_NUM; i++) {
                setDefectsNum(getDefectsNum() + recDataPtr->m_pDefect[i].size());
            }
            setDefectEchoRecord(shared_ptr<defectRecordModel[]>(new defectRecordModel[getDefectsNum()]));
            setBottomWaveAttenuationRecord(shared_ptr<defectRecordModel[]>(new defectRecordModel[getDefectsNum()]));
            for (uint32_t ch = 0, idx = 0; ch < HD_CHANNEL_NUM; ch++) {
                for (uint32_t i = 0; i < recDataPtr->m_pDefect[ch].size(); i++, idx++) {
                    // 缺陷回波记录
                    defectEchoRecord[idx].axial      = recDataPtr->m_pDefect[ch][i]->nAxialDepth;
                    defectEchoRecord[idx].radial     = recDataPtr->m_pDefect[ch][i]->nRadialDistance;
                    defectEchoRecord[idx].gain       = recDataPtr->m_pDefect[ch][i]->nParam1; // TODO: 获取增益
                    defectEchoRecord[idx].waveHeight = recDataPtr->m_pDefect[ch][i]->nWaveHeight;
                    defectEchoRecord[idx].dBDiff     = recDataPtr->m_pDefect[ch][i]->nDBOffset;

                    // TODO: 底波衰减记录
                    bottomWaveAttenuationRecord[idx].axial      = recDataPtr->m_pDefect[ch][i]->nAxialDepth;
                    bottomWaveAttenuationRecord[idx].radial     = recDataPtr->m_pDefect[ch][i]->nRadialDistance;
                    bottomWaveAttenuationRecord[idx].gain       = recDataPtr->m_pDefect[ch][i]->nParam1; // TODO: 获取增益
                    bottomWaveAttenuationRecord[idx].waveHeight = recDataPtr->m_pDefect[ch][i]->nWaveHeight;
                    bottomWaveAttenuationRecord[idx].dBDiff     = recDataPtr->m_pDefect[ch][i]->nDBOffset;
                }
            }

            return true;
        }

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
        Q_PROPERTY(SP_DefModel defectEchoRecord READ getDefectEchoRecord WRITE setDefectEchoRecord NOTIFY defectEchoRecordChanged)
        Q_PROPERTY(SP_DefModel bottomWaveAttenuationRecord READ getBottomWaveAttenuationRecord WRITE setBottomWaveAttenuationRecord NOTIFY bottomWaveAttenuationRecordChanged)
        Q_PROPERTY(int defectsNum READ getDefectsNum WRITE setDefectsNum NOTIFY defectsNumChanged)
    };

} // namespace Ruitie
