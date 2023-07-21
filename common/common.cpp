#include "common.h"
#include "../morose_config.h"

#include "msg/dailyrecordmsg.h"
#include "msg/discoveryrecordmsg.h"
#include "msg/inspectionrecordmsg.h"
#include "msg/quarterlyrecordmsg.h"
#include "views/datatablemodel.h"
#include "views/defectrecordview.h"
#include "views/inspectionrecordview.h"
#include "views/performancerecordview.h"
#include "views/qwaveview.h"

void Morose::logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static QMutex mutex;
    static int    maxCatelogyLen = 0;
    int           categoryLen    = QString(context.category).length();
    static int    maxMsgLen      = 0;
    int           msgLen         = msg.length();
    if (categoryLen > maxCatelogyLen) {
        maxCatelogyLen = categoryLen;
    }
    if (msgLen > maxMsgLen) {
        maxMsgLen = msgLen;
    }
    QString logLevel;
    QString logCategory = QString("[%1]%2").arg(QString(context.category), QString(maxCatelogyLen - categoryLen, ' '));
    QString logLineInfo = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString logTimeInfo = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz]");

    switch (type) {
        case QtDebugMsg: {
            logLevel = QString("[Debug]   ");
            break;
        }
        case QtWarningMsg: {
            logLevel = QString("[Warning] ");
            break;
        }
        case QtCriticalMsg: {
            logLevel = QString("[Critical]");
            break;
        }
        case QtFatalMsg: {
            logLevel = QString("[Fatal]   ");
            break;
        }
        case QtInfoMsg: {
            logLevel = QString("[Info]    ");
            break;
        }
    }
    QString message = QString("%1 %2 %3 \t %4 \t\t-- %5\r\n")
                          .arg(logTimeInfo, logLevel, logCategory, QString("%1%2").arg(msg, QString(maxMsgLen - msgLen, ' ')), logLineInfo);
    QFile file("./log/log.txt");
    mutex.lock();
    if (file.size() > 1024 * 1024 * 4) {
        QFile::copy("log/log.txt", "log/log0.txt");
        file.resize(0);
    }
    file.open(QFile::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    if (type == QtCriticalMsg || type == QtFatalMsg) {
        QFile file("./log/error.txt");
        file.open(QFile::WriteOnly | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << message;
        file.flush();
        file.close();
    }
    text_stream << message;
    file.flush();
    file.close();
    mutex.unlock();
}

void Morose::registerVariable(QQmlContext *context) {
    context->setContextProperty("MOROSE_APP_VERSION", APP_VERSION);
    context->setContextProperty("MOROSE_GIT_REPOSITORY", GIT_REPOSITORY);
    context->setContextProperty("MOROSE_GIT_USER_NAME", GIT_USER_NAME);
    context->setContextProperty("MOROSE_GIT_USER_EMAIL", GIT_USER_EMAIL);
    qmlRegisterType<QWaveView>("mengyou.WaveView", 1, 0, "WaveView");
    qmlRegisterType<DataTableModel>("mengyou.DataTableModel", 1, 0, "DataTableModel");
    qmlRegisterType<Ruitie::DiscoveryRecordMsg>("mengyou.Model", 1, 0, "DiscoveryRecordMsg");
    qmlRegisterType<Ruitie::InspectionRecordMsg>("mengyou.Model", 1, 0, "InspectionRecordMsg");
    qmlRegisterType<Ruitie::QuarterlyRecordMsg>("mengyou.Model", 1, 0, "QuarterlyRecordMsg");
    qmlRegisterType<Ruitie::DailyRecordMsg>("mengyou.Model", 1, 0, "DailyRecordMsg");
    qmlRegisterType<Ruitie::DefectRecordView>("mengyou.Model", 1, 0, "DefectRecordView");
    qmlRegisterType<Ruitie::InspectionRecordView>("mengyou.Model", 1, 0, "InspectionRecordView");
    qmlRegisterType<Ruitie::PerformanceRecordView>("mengyou.Model", 1, 0, "PerformanceRecordView");
    qmlRegisterType<Ruitie::ExcelRunner>("mengyou.Model", 1, 0, "ExcelRunner");
}

#ifdef MOROSE_TEST

    #include "ruitiedefine.h"

void testSaveFile() {
    using namespace Ruitie;
    using namespace std;
    // dailyrecord
    auto constexpr tableSize      = 5;
    SYS_PARA      sysPara         = {};
    CH_PARA       chPara          = {};
    PLC_SCAN_PAPA plcScanPara     = {};
    WHEEL_PAPA    wheelPara       = {};
    DB_USER_DATA  dbUserData      = {};
    DB_SCAN_DATA  dbScanData      = {};
    dbScanData.nTotalDefectNum    = tableSize;
    dbScanData.lTotalScanSize     = tableSize;
    DB_QUARTER_DATA dbQuarterData = {};

    struct nDB_DEFECT_DATA {
        DB_DEFECT_DATA dbDefectData[tableSize];
    };
    struct nDB_SCAN_DATA {
        DB_SCAN_DATA nDbScanData[tableSize];
    };

    nDB_DEFECT_DATA ndbDefectData = {};

    nDB_SCAN_DATA nDbScanData = {};

    for (int i = 0, j = 0; i < dbScanData.nTotalDefectNum; i++) {
        ndbDefectData.dbDefectData[i].nAxialDepth     = j++;
        ndbDefectData.dbDefectData[i].nRadialDistance = j++;
        ndbDefectData.dbDefectData[i].nWaveHeight     = j++;

        strcpy_s(nDbScanData.nDbScanData[i].szWheelNumber, QString::number(j++).toStdString().c_str());
    }
    strcpy_s(sysPara.szUseOrg, "瑞铁");
    strcpy_s(sysPara.szMadeModal, "u230718");
    strcpy_s(sysPara.szMadeSerial, "23718");
    strcpy_s(sysPara.szMadeFact, "Union");
    strcpy_s(sysPara.szMadeDate, "2023-7-18");
    strcpy_s(dbQuarterData.szProbeType, "水浸探头");
    dbQuarterData.nParam2              = 114514;
    dbQuarterData.m_nSensitivityMargin = 9527;
    for (int i = 0, j = 0; i < 10; i++) {
        dbQuarterData.m_nDistinguishValuel[i] = ++j;
        dbQuarterData.m_nHorLinearity[i]      = ++j;
        dbQuarterData.m_nVerLinearity[i]      = ++j;
        dbQuarterData.m_nDynamicRange[i]      = ++j;
    }
    saveFile("dailyRecord.bin", &sysPara, &chPara, &plcScanPara, &wheelPara, &dbUserData, &dbScanData, &ndbDefectData, &nDbScanData);
    saveFile("discoveryRecord.bin", &sysPara, &chPara, &plcScanPara, &wheelPara, &dbUserData, &dbScanData, &ndbDefectData, &nDbScanData);
    saveFile("inspectionRecord.bin", &sysPara, &chPara, &plcScanPara, &wheelPara, &dbUserData, &dbScanData, &ndbDefectData, &nDbScanData);
    saveFile("quarterlyRecord.bin", &sysPara, &dbQuarterData, &dbUserData);
}

void Morose::test(void) {
    testSaveFile();
}

#endif
