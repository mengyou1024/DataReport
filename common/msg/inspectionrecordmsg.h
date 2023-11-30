#pragma once

#include "../ruitie.h"
#include "../ruitiedefine.h"
#include <QDir>
#include <QRegularExpression>

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
        Q_INVOKABLE bool loadFile([[maybe_unused]] QString fileName) {
            QDir dir(fileName);
            if (!dir.exists()) {
                return false;
            }

            shared_ptr<RecData> lastDataPtr = nullptr;
            dir.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list = dir.entryInfoList();
            setRecordNum(list.size());
            setRecordPtr(shared_ptr<InspectionRecordModel[]>(new InspectionRecordModel[recordNum]));
            int index = 0;
            for (const auto &file : list) {
                qInfo() << file.fileName();
                QString ffN        = fileName + "/" + file.fileName();
                auto    recDataPtr = DataLoader::getRecData(ffN);
                lastDataPtr        = recDataPtr;
                int defectsNum     = 0;

                qreal   radial      = 0.0;
                qreal   axial       = 0.0;
                qreal   waveHeight  = 0.0;
                qreal   dbDiff      = 0.0;
                QString result      = "";
                qreal   sensitivity = 0.0;

                for (uint32_t ch = 0, idx = 0; ch < HD_CHANNEL_NUM; ch++) {
                    defectsNum += recDataPtr->m_pDefect[ch].size();
                    for (uint32_t i = 0; i < recDataPtr->m_pDefect[ch].size(); i++, idx++) {
                        if (recDataPtr->m_pDefect[ch][i]->nDBOffset > dbDiff) {
                            radial      = recDataPtr->m_pDefect[ch][i]->nRadialDistance;
                            axial       = recDataPtr->m_pDefect[ch][i]->nAxialDepth;
                            waveHeight  = recDataPtr->m_pDefect[ch][i]->nWaveHeight / 2.55;
                            dbDiff      = recDataPtr->m_pDefect[ch][i]->nDBOffset;
                            result      = recDataPtr->m_pDefect[ch][i]->bDefectType ? "透声不良" : "缺陷";
                            sensitivity = recDataPtr->m_pDefect[ch][i]->nSensitivity;
                        }
                    }
                }

                recordPtr[index].wheelType   = QString::fromStdWString(recDataPtr->wheelParam.szWheelType);
                recordPtr[index].heatSerial  = QString::fromStdWString(recDataPtr->wheelParam.szHeatNumber);
                recordPtr[index].wheelSerial = QString::fromStdWString(recDataPtr->wheelParam.szWheelNumber);
                recordPtr[index].defectsNum  = defectsNum;
                recordPtr[index].radial      = radial;
                recordPtr[index].axial       = axial;
                recordPtr[index].waveHeight  = waveHeight;
                recordPtr[index].dBDiff      = dbDiff;
                recordPtr[index].result      = result; // TODO: result
                recordPtr[index].sensitivity = sensitivity;

                index++;
            }

            const static QRegularExpression rexp(R"(^.*/(\d{4})(\d+)/(\d+)/$)");
            auto                            match    = rexp.match(fileName);
            QDateTime                       dateTime = QDateTime::currentDateTime();
            if (match.hasMatch()) {
                auto dataStr = QString("%1-%2-%3").arg(match.captured(1), match.captured(2), match.captured(3));
                dateTime     = QDateTime::fromString(dataStr, "yyyy-M-d");
            }
            if (lastDataPtr != nullptr) {
                auto recDataPtr = lastDataPtr;
                setCompanyName(QString::fromWCharArray(recDataPtr->wheelParam.szDetectionFact));
                setDetectDate(dateTime.toString("yyyy-M-d"));
                setDeviceName(QString::fromWCharArray(recDataPtr->wheelParam.szDeviceName));
                setDeviceType(QString::fromWCharArray(L""));   // TODO: 设备型号保存位置
                setDeviceSerial(QString::fromWCharArray(L"")); // TODO: 设备编号保存位置
                setDetectStandard(QString::fromWCharArray(recDataPtr->wheelParam.szDetectionStd));
                setDetectContent(QString::fromWCharArray(recDataPtr->wheelParam.szDetectionContent));
                setDetectRegion(QString::fromWCharArray(recDataPtr->wheelParam.szDetectionArea));
                setProbe(QString::fromWCharArray(L"")); // TODO: 探头
                setSoundSpeed(recDataPtr->paramChannel[0].m_iVelocity);
            }
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
