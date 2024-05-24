#pragma once

#include "../ruitie.h"
#include "../ruitiedefine.h"
#include <QRegularExpression>

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
        int         bottomNum                   = 0;       ///< 底波衰减数量
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
            const static QRegularExpression rexp(R"(^.*/(\d{4})(\d+)/(\d+)/[^/]*$)");
            auto                            match    = rexp.match(fileName);
            QDateTime                       dateTime = QDateTime::currentDateTime();
            if (match.hasMatch()) {
                qInfo() << match.captured(1) << match.captured(2) << match.captured(3);
                auto dataStr = QString("%1-%2-%3").arg(match.captured(1), match.captured(2), match.captured(3));
                dateTime     = QDateTime::fromString(dataStr, "yyyy-M-d");
            }
            // 解析数据
            setCompanyName(QString::fromWCharArray(recDataPtr->wheelParam.szDetectionFact));
            setDetectDate(dateTime.toString("yyyy-M-d"));
            setWorkFreq(recDataPtr->paramSystem.Frequency);   // TODO: 探头频率保存位置
            setProbe(QString::fromWCharArray(L""));           // TODO: 探头保存位置
            setCoupledMode(QString::fromWCharArray(L"水浸")); // TODO: 耦合方式
            setWheelType(QString::fromWCharArray(recDataPtr->wheelParam.szWheelModel));
            setHeatSerial(QString::fromWCharArray(recDataPtr->wheelParam.szHeatNumber));
            setWheelSerial(QString::fromWCharArray(recDataPtr->wheelParam.szWheelNumber));
            setDefectsNum(0);

            int _defectsNum = 0;
            int _bottomNum  = 0;
            // 填充缺陷数据
            for (uint32_t i = 0; i < HD_CHANNEL_NUM; i++) {
                for (const auto &defect : recDataPtr->m_pDefect[i]) {
                    if (defect->bDefectType == 0) {
                        _defectsNum += 1;
                    } else {
                        _bottomNum += 1;
                    }
                }
            }
            setBottomNum(_bottomNum);
            setDefectsNum(_defectsNum);

            setDefectEchoRecord(shared_ptr<defectRecordModel[]>(new defectRecordModel[getDefectsNum()]));
            setBottomWaveAttenuationRecord(shared_ptr<defectRecordModel[]>(new defectRecordModel[getBottomNum()]));
            for (uint32_t ch = 0, idx_echo = 0, idx_bottom = 0; ch < HD_CHANNEL_NUM; ch++) {
                for (uint32_t i = 0; i < recDataPtr->m_pDefect[ch].size(); i++) {
                    if (recDataPtr->m_pDefect[ch][i]->bDefectType == 0) {
                        // 缺陷回波记录
                        defectEchoRecord[idx_echo].axial      = recDataPtr->m_pDefect[ch][i]->nAxialDepth;
                        defectEchoRecord[idx_echo].radial     = recDataPtr->m_pDefect[ch][i]->nRadialDistance;
                        defectEchoRecord[idx_echo].gain       = recDataPtr->m_pDefect[ch][i]->nSensitivity;
                        defectEchoRecord[idx_echo].waveHeight = recDataPtr->m_pDefect[ch][i]->nWaveHeight;
                        defectEchoRecord[idx_echo].dBDiff     = recDataPtr->m_pDefect[ch][i]->nDBOffset;
                        idx_echo += 1;
                    } else {
                        // 底波衰减记录
                        bottomWaveAttenuationRecord[idx_bottom].axial      = recDataPtr->m_pDefect[ch][i]->nAxialDepth;
                        bottomWaveAttenuationRecord[idx_bottom].radial     = recDataPtr->m_pDefect[ch][i]->nRadialDistance;
                        bottomWaveAttenuationRecord[idx_bottom].gain       = recDataPtr->m_pDefect[ch][i]->nSensitivity;
                        bottomWaveAttenuationRecord[idx_bottom].waveHeight = recDataPtr->m_pDefect[ch][i]->nWaveHeight;
                        bottomWaveAttenuationRecord[idx_bottom].dBDiff     = recDataPtr->m_pDefect[ch][i]->nDBOffset;
                        idx_bottom += 1;
                    }
                }
            }

            return true;
        }

        int  getBottomNum() const;
        void setBottomNum(int newBottomNum);

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
        void bottomNumChanged();

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
        Q_PROPERTY(int bottomNum READ getBottomNum WRITE setBottomNum NOTIFY bottomNumChanged)
    };

} // namespace Ruitie
