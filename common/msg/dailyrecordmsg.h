#pragma once

#include "../ruitie.h"
#include "../ruitiedefine.h"

namespace Ruitie {
    /**
     * @brief 日常性能检查表信息
     */
    class DailyRecordMsg : public QObject {
        Q_OBJECT
    public:
        using SP_DefModel = shared_ptr<defectRecordModel[]>;
        DailyRecordMsg(QObject *parent = nullptr) {}

        QString     companyName = "";      ///< 单位名称
        QString     detectDate  = "";      ///< 探伤日期
        QString     detectDeive = "";      ///< 探伤设备
        qreal       workFreq    = 0;       ///< 工作频率
        QString     probe       = "";      ///< 探头
        QString     coupledMode = "";      ///< 耦合方式
        QString     wheelType   = "";      ///< 车轮型号
        QString     heatSerial  = "";      ///< 炉号
        QString     wheelSerial = "";      ///< 车轮编号
        int         defectsNum  = 0;       ///< 缺陷数目
        SP_DefModel defectsPtr  = nullptr; ///< 缺陷指针

        QString     getCompanyName() const;
        void        setCompanyName(const QString &newCompanyName);
        QString     getDetectDeive() const;
        void        setDetectDeive(const QString &newDetectDeive);
        qreal       getWorkFreq() const;
        void        setWorkFreq(qreal newWorkFreq);
        QString     getProbe() const;
        void        setProbe(const QString &newProbe);
        QString     getCoupledMode() const;
        void        setCoupledMode(const QString &newCoupledMode);
        QString     getWheelType() const;
        void        setWheelType(const QString &newWheelType);
        QString     getHeatSerial() const;
        void        setHeatSerial(const QString &newHeatSerial);
        QString     getWheelSerial() const;
        void        setWheelSerial(const QString &newWheelSerial);
        QString     getDetectDate() const;
        void        setDetectDate(const QString &newDetectDate);
        void        setDefectsPtr(const SP_DefModel &newDefectsPtr);
        SP_DefModel getDefectsPtr() const;

        int  getDefectsNum() const;
        void setDefectsNum(int newDefectsNum);

        Q_INVOKABLE bool saveFile(QString fileName) {
            return Ruitie::saveFile(fileName, this);
        }

        Q_INVOKABLE bool loadFile(QString fileName) {
            auto recDataPtr = DataLoader::getRecData(fileName);
            if (recDataPtr == nullptr) {
                return false;
            }

            // 解析数据
            setCompanyName(QString::fromStdWString(L"江苏瑞铁轨道装备股份有限公司"));
            setDetectDate(QDateTime::currentDateTime().toString("yyyy-M-d"));
            setDetectDeive(QString::fromStdWString(L"(水浸)车轮超声波探伤机"));
            setWorkFreq(2.5);
            setProbe(QString::fromStdWString(L"探头"));
            setCoupledMode(QString::fromStdWString(L"水浸"));
            setWheelType(QString::fromStdWString(L"车轮类型"));
            setHeatSerial(QString::fromStdWString(L"炉号"));
            setWheelSerial(QString::fromStdWString(L"车轮编号"));

            // 填充缺陷数据
            for (uint32_t i = 0; i < HD_CHANNEL_NUM; i++) {
                setDefectsNum(getDefectsNum() + recDataPtr->m_pDefect[i].size());
            }
            setDefectsPtr(shared_ptr<defectRecordModel[]>(new defectRecordModel[getDefectsNum()]));

            for (uint32_t ch = 0, idx = 0; ch < HD_CHANNEL_NUM; ch++) {
                for (uint32_t i = 0; i < recDataPtr->m_pDefect[ch].size(); i++, idx++) {
                    defectsPtr[idx].axial      = recDataPtr->m_pDefect[ch][i]->nAxialDepth;
                    defectsPtr[idx].radial     = recDataPtr->m_pDefect[ch][i]->nRadialDistance;
                    defectsPtr[idx].gain       = recDataPtr->m_pDefect[ch][i]->nParam1; // TODO: 获取增益
                    defectsPtr[idx].waveHeight = recDataPtr->m_pDefect[ch][i]->nWaveHeight;
                    defectsPtr[idx].dBDiff     = recDataPtr->m_pDefect[ch][i]->nDBOffset;
                }
            }

            return true;
        }

    signals:
        void companyNameChanged();
        void detectDateChanged();
        void detectDeiveChanged();
        void workFreqChanged();
        void probeChanged();
        void coupledModeChanged();
        void wheelTypeChanged();
        void heatSerialChanged();
        void wheelSerialChanged();
        void defectsPtrChanged();
        void defectsNumChanged();

    private:
        Q_PROPERTY(QString companyName READ getCompanyName WRITE setCompanyName NOTIFY companyNameChanged)
        Q_PROPERTY(QString detectDeive READ getDetectDeive WRITE setDetectDeive NOTIFY detectDeiveChanged)
        Q_PROPERTY(qreal workFreq READ getWorkFreq WRITE setWorkFreq NOTIFY workFreqChanged)
        Q_PROPERTY(QString probe READ getProbe WRITE setProbe NOTIFY probeChanged)
        Q_PROPERTY(QString coupledMode READ getCoupledMode WRITE setCoupledMode NOTIFY coupledModeChanged)
        Q_PROPERTY(QString wheelType READ getWheelType WRITE setWheelType NOTIFY wheelTypeChanged)
        Q_PROPERTY(QString heatSerial READ getHeatSerial WRITE setHeatSerial NOTIFY heatSerialChanged)
        Q_PROPERTY(QString wheelSerial READ getWheelSerial WRITE setWheelSerial NOTIFY wheelSerialChanged)
        Q_PROPERTY(QString detectDate READ getDetectDate WRITE setDetectDate NOTIFY detectDateChanged)
        Q_PROPERTY(SP_DefModel defectsPtr READ getDefectsPtr WRITE setDefectsPtr NOTIFY defectsPtrChanged)
        Q_PROPERTY(int defectsNum READ getDefectsNum WRITE setDefectsNum NOTIFY defectsNumChanged)
    };

} // namespace Ruitie
