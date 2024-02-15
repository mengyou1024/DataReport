#include "common/common.h"
#include "morose_config.h"
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFont>
#include <QGuiApplication>
#include <QPluginLoader>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <SingleApplication.h>

Q_LOGGING_CATEGORY(TAG_Main, "main");

int main(int argc, char *argv[]) {
    // 注册日志处理回调函数
    qInstallMessageHandler(Morose::logMessageHandler);

    SingleApplication     app(argc, argv);
    QQmlApplicationEngine engine;
    QSettings             logSetting("log/setting.ini", QSettings::IniFormat);

    // SingleApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Round);

    logSetting.beginGroup("Rules");
    auto filter = logSetting.value("filterRules");
    if (filter.isNull()) {
#if defined(QT_DEBUG) || defined(MOROSE_TEST)
        logSetting.setValue("filterRules", "");
#else
        logSetting.setValue("filterRules", "*.debug=false");
#endif
        filter = logSetting.value("filterRules");
    }
    QLoggingCategory::setFilterRules(filter.toString());
    logSetting.endGroup();
    qInfo(TAG_Main) << QString(100, '-').toStdString().c_str();

    // 设置默认字体
    QFont font = QFont(QString::fromLocal8Bit("楷体"), 14);
    app.setFont(font);

    // 加载QML、注册环境变量
    const QUrl url("qrc:/resource/qml/main.qml");
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    // 获取程序传入参数
    QCoreApplication::setApplicationName(APP_VERSION);
    QCommandLineParser parser;

    parser.setApplicationDescription("DataReport");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption optDiscovery("discovery", "发现缺陷记录表的文件路径\t(默认值为: discoveryRecord.bin).", "discoveryFile", "discoveryRecord.bin");
    QCommandLineOption optInspection("inspection", "探伤记录表的文件路径\t(默认值为: inspectionRecord.bin)).", "inspectionFile", "inspectionRecord.bin");
    QCommandLineOption optDaily("daily", "日常性能检查记录表的文件路径\t(默认值为: dailyRecord.bin).", "dailyFile", "dailyRecord.bin");
    QCommandLineOption optQuarterly("quarterlyRecord", "季度性能检查记录表的文件路径\t(默认值为: quarterlyRecord.bin)", "quarterlyFile", "quarterlyRecord.bin");

    parser.addOption(optDiscovery);
    parser.addOption(optInspection);
    parser.addOption(optDaily);
    parser.addOption(optQuarterly);
    parser.process(app);

    engine.rootContext()->setContextProperty("mainDiscoveryFile", parser.value(optDiscovery));
    engine.rootContext()->setContextProperty("mainInspectionFile", parser.value(optInspection));
    engine.rootContext()->setContextProperty("mainDailyFile", parser.value(optDaily));
    engine.rootContext()->setContextProperty("mainQuarterlyFile", parser.value(optQuarterly));

    qInfo(TAG_Main) << "DataReport run startup, application version:" << APP_VERSION;
    Morose::registerVariable(engine.rootContext());
#ifdef MOROSE_TEST
    Morose::test();
#endif
    engine.load(url);

    auto rootObjs   = engine.rootObjects();
    auto mainWindow = qobject_cast<QQuickWindow *>(rootObjs.first());
    QObject::connect(&app, &SingleApplication::instanceStarted, mainWindow, [mainWindow]() {
        mainWindow->setFlag(Qt::WindowStaysOnTopHint, true);
        if (mainWindow->windowState() == Qt::WindowMaximized) {
            mainWindow->resize(mainWindow->minimumSize());
            mainWindow->setX((QGuiApplication::primaryScreen()->geometry().width() - mainWindow->minimumSize().width()) / 2);
            mainWindow->setY((QGuiApplication::primaryScreen()->geometry().height() - mainWindow->minimumSize().height()) / 2);
        }
        mainWindow->showNormal();
        mainWindow->setFlag(Qt::WindowStaysOnTopHint, false);
        mainWindow->requestActivate();
        mainWindow->raise();
    });

    return app.exec();
}
