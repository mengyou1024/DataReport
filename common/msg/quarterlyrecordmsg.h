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
            // 解析数据
            SYS_PARA        sysPara;
            DB_QUARTER_DATA dbQuarterData;
            DB_USER_DATA    dbUserData;
            READ_DATA(sysPara);
            READ_DATA(dbQuarterData);
            READ_DATA(dbUserData);
            setCompanyName(QString(sysPara.szUseOrg));
            setPrintDate("TODO:printDate");
            setInstrumentType(QString(sysPara.szMadeModal));
            setInstrumentSerial(QString(sysPara.szMadeSerial));
            setManufactureCompany(QString(sysPara.szMadeFact));
            setManufactureDate(QString(sysPara.szMadeDate));
            for (int i = 0; i < 5; i++) {
                probeType[i]   = QString(dbQuarterData.szProbeType);
                probeSerial[i] = QString::number(dbQuarterData.nParam2);
            }
            for (int i = 0; i < 10; i++) {
                performance[i][0] = QString::number(dbQuarterData.m_nSensitivityMargin);
                performance[i][1] = QString::number(dbQuarterData.m_nDistinguishValuel[i]);
                performance[i][2] = QString::number(dbQuarterData.m_nHorLinearity[i]);
                performance[i][3] = QString::number(dbQuarterData.m_nVerLinearity[i]);
                performance[i][4] = QString::number(dbQuarterData.m_nDynamicRange[i]);
            }

#undef READ_DATA

            file.close();
            return true;
        }

        QString getPrintDate() const;
        void    setPrintDate(const QString &newPrintDate);

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
