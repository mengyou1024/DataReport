#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QQmlContext>
#include <QtCore>
#include <QtQml>

namespace Morose {
    void         logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    QJsonObject &loadGlobalEnvironment();
    QJsonObject &getGlobalEnvironment();
    void         registerVariable(QQmlContext *context);
#ifdef MOROSE_TEST
    void test(void);
#endif
} // namespace Morose
