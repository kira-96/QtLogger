#include <QCoreApplication>
#include <logger.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qInstallMessageHandler(Logger::outputLogMessage);
    Logger::getInstance()->setLogLevel(DEBUG);

    qDebug("debug");
    qInfo("测试");
    qWarning("warning");
    qCritical("error");
    qFatal("fatal");

    return a.exec();
}
