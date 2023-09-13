#include "common.h"
#include "../morose_config.h"

#include "msg/dailyrecordmsg.h"
#include "msg/discoveryrecordmsg.h"
#include "msg/inspectionrecordmsg.h"
#include "msg/quarterlyrecordmsg.h"
#include "qframelesswindow.h"
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

#if defined(QT_DEBUG)
    QString message = QString("%2 %3 \t %4\r\n").arg(logLevel, logCategory, QString("%1%2").arg(msg, QString(maxMsgLen - msgLen, ' ')));
    mutex.lock();
    printf(message.toLocal8Bit());
    fflush(stdout);
    mutex.unlock();
#else
    QString logLineInfo = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString logTimeInfo = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz]");
    QString message     = QString("%1 %2 %3 \t %4 \t\t-- %5\r\n").arg(logTimeInfo, logLevel, logCategory, QString("%1%2").arg(msg, QString(maxMsgLen - msgLen, ' ')), logLineInfo);
    QFile   file("./log/log.txt");
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
#endif
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
    qmlRegisterType<QFramelessWindow>("mengyou.Model", 1, 0, "QFramelessWindow");
    auto    json    = loadGlobalEnvironment();
    auto    dataDir = json["dataDir"];
    QString dirStr  = "";
    if (dataDir.isString()) {
        QUrl url(dataDir.toString());
        if (url.isRelative()) {
            dirStr = QCoreApplication::applicationDirPath() + "/" + url.toString();
        } else {
            dirStr = url.toString();
        }
    }
    qInfo(TAG_Ruitie) << "APP_DATA_DIR:" << dirStr;
    context->setContextProperty("APP_DATA_DIR", dirStr);
}

#ifdef MOROSE_TEST

    #include "ruitiedefine.h"

void Morose::test(void) {
    Ruitie::RecData data;
    qint64          time = QDateTime::currentMSecsSinceEpoch();
    Ruitie::loadFile(R"(C:\Users\Administrator\Desktop\Mutilple\x64\Debug\Data\Scan\202308\30\09-44-22.scdat)", data);
    qInfo(TAG_Ruitie) << "loadFile spend time:" << QDateTime::currentMSecsSinceEpoch() - time;
    qInfo(TAG_Ruitie) << "data is valid:" << data.isValid();
}

#endif

QJsonObject &Morose::getGlobalEnvironment() {
    static QJsonObject obj;
    return obj;
}

QJsonObject &Morose::loadGlobalEnvironment() {
    QFile file("Config.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) {
            getGlobalEnvironment() = doc.object();
        }
        file.close();
    }
    return getGlobalEnvironment();
}
