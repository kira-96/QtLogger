#include "logger.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMutexLocker>
#include <QRegularExpression>
#include <QTextStream>

Logger::Logger(QObject *parent)
    : QObject(parent)
    , log_level(INFO)
{
    log_dir = QCoreApplication::applicationDirPath() + "/log";
    QDir dir(log_dir);

    if (!dir.exists())
    {
        dir.mkpath(log_dir);
    }
}

Logger::~Logger()
{
}

Logger* Logger::getInstance()
{
    static Logger instance;
    return &instance;
}

void Logger::outputLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    getInstance()->outputMessage(type, context, message);
}

quint8 Logger::logLevel(QtMsgType type)
{
    switch (type)
    {
    case QtDebugMsg: return DEBUG;
    case QtInfoMsg: return INFO;
    case QtWarningMsg: return WARN;
    case QtCriticalMsg: return CRITI;
    case QtFatalMsg: return FATAL;
    default: return NOLOG;
    }
}

void Logger::setLogLevel(quint8 level)
{
    log_level = level;
}

const QString Logger::outputLevel(quint8 level)
{
    switch (level)
    {
    case DEBUG: return " [DEBUG] ";
    case INFO:  return " [INFO ] ";
    case WARN:  return " [WARN ] ";
    case CRITI: return " [CRITI] ";
    case FATAL: return " [FATAL] ";
    default: return " ";
    }
}

const QString Logger::outputFunction(const char *function)
{
    QRegularExpression regexp("\\w+::\\w+");  // 匹配类函数
    QRegularExpressionMatch match = regexp.match(function);

    if (match.hasMatch())
    {
        return QString("<%1> ").arg(match.captured(0));
    }

    regexp = QRegularExpression(" \\w+\\(");  // 匹配全局函数
    match = regexp.match(function);

    if (match.hasMatch())
    {
        return QString("<%1> ").arg(match.captured(0).remove('(').trimmed());
    }
    else
    {
        return "<> ";
    }
}

void Logger::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    QMutexLocker locker(&mutex);

    quint8 level = logLevel(type);

    emit Log(level, message);

    if (level < log_level)
    {
        return;
    }

    QDateTime dt = QDateTime::currentDateTime();
    QFile file(log_dir + dt.toString("/yyyy-MM-dd-HH.log"));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        emit Log(CRITI, "Write log error! " + file.errorString() + file.fileName());
        return;
    }

    QTextStream stream(&file);
    stream.setCodec("utf-8");  // 统一使用utf-8编码
    stream << dt.toString("HH:mm:ss") << outputLevel(level) << outputFunction(context.function) << message << endl;
    file.flush();
    file.close();
}
