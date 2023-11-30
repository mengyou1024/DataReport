#pragma once

#include "../ruitie.h"
#include "../ruitiedefine.h"

namespace Ruitie {
    /**
     * @brief 季度性能检查表信息
     */
    class QuarterlyRecordMsg : public QObject {
        Q_OBJECT
    public:
        explicit QuarterlyRecordMsg(QObject *parent = nullptr) {}

        QString companyName        = ""; ///< 单位名称
        QString printDate          = ""; ///< 打印日期
        QString instrumentType     = ""; ///< 仪器型号
        QString instrumentSerial   = ""; ///< 仪器编号
        QString manufactureCompany = ""; ///< 制造单位
        QString manufactureDate    = ""; ///< 制造日期
        QString probeType[5]       = {}; ///< 探头型号
        QString probeSerial[5]     = {}; ///< 探头编号
        QString performance[10][5] = {}; ///< 性能 A[1:4] B1 C1 D[1:4]

    public:
        QString getCompanyName() const;
        void    setCompanyName(const QString &newCompanyName);
        QString getInstrumentType() const;
        void    setInstrumentType(const QString &newInstrumentType);
        QString getInstrumentSerial() const;
        void    setInstrumentSerial(const QString &newInstrumentSerial);
        QString getManufactureCompany() const;
        void    setManufactureCompany(const QString &newManufactureCompany);
        QString getManufactureDate() const;
        void    setManufactureDate(const QString &newManufactureDate);
        QString getPrintDate() const;
        void    setPrintDate(const QString &newPrintDate);

        Q_INVOKABLE bool saveFile(QString fileName) {
            return Ruitie::saveFile(fileName, this);
        }

        Q_INVOKABLE bool loadFile(QString fileName) {
            QFile file(fileName);
            if (!file.exists()) {
                qCritical(TAG_Ruitie) << "file:" << fileName << "is not exsist";
                return false;
            }
            if (!file.open(QFile::ReadOnly)) {
                qCritical(TAG_Ruitie()) << "file:" << fileName << "open failed";
                return false;
            }
            DB_QUARTER_DATA quartData = {};
            file.read((char *)&quartData, sizeof(DB_QUARTER_DATA));
            file.close();
            // 解析数据
            setCompanyName(QString::fromStdWString(L"江苏瑞铁轨道装备股份有限公司"));
            setPrintDate(QDateTime::currentDateTime().toString("yyyy-M-d"));
            setInstrumentType(QString::fromStdWString(L""));   // TODO: 仪器型号
            setInstrumentSerial(QString::fromStdWString(L"")); // TODO: 仪器编号
            setManufactureCompany(QString::fromStdWString(L"江苏瑞铁轨道装备股份有限公司"));
            setManufactureDate(QString(quartData.m_dtTime));
            for (int i = 0; i < 5; i++) {
                probeType[i]   = QString::fromStdWString(L"");
                probeSerial[i] = QString::number(0);
            }
            for (int i = 0; i < 10; i++) {
                performance[i][0] = QString::number(quartData.m_nSensitivityMargin[i], 'f', 1);
                performance[i][1] = QString::number(quartData.m_nDistinguishValuel[i], 'f', 1);
                performance[i][2] = QString::number(quartData.m_nHorLinearity[i], 'f', 1);
                performance[i][3] = QString::number(quartData.m_nVerLinearity[i], 'f', 1);
                performance[i][4] = QString::number(quartData.m_nDynamicRange[i], 'f', 1);
            }
            return true;
        }

    signals:
        void companyNameChanged();
        void instrumentTypeChanged();
        void instrumentSerialChanged();
        void manufactureCompanyChanged();
        void manufactureDateChanged();

        void printDateChanged();

    private:
        Q_PROPERTY(QString companyName READ getCompanyName WRITE setCompanyName NOTIFY companyNameChanged)
        Q_PROPERTY(QString instrumentType READ getInstrumentType WRITE setInstrumentType NOTIFY instrumentTypeChanged)
        Q_PROPERTY(QString instrumentSerial READ getInstrumentSerial WRITE setInstrumentSerial NOTIFY instrumentSerialChanged)
        Q_PROPERTY(QString manufactureCompany READ getManufactureCompany WRITE setManufactureCompany NOTIFY manufactureCompanyChanged)
        Q_PROPERTY(QString manufactureDate READ getManufactureDate WRITE setManufactureDate NOTIFY manufactureDateChanged)
        Q_PROPERTY(QString printDate READ getPrintDate WRITE setPrintDate NOTIFY printDateChanged)
    };

} // namespace Ruitie
