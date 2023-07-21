#pragma once

#include <QProcess>
#include <QQmlContext>
#include <QtCore>
#include <QtQml>

namespace Morose {
    void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void registerVariable(QQmlContext *context);
#ifdef MOROSE_TEST
    void test(void);
#endif
} // namespace Morose
