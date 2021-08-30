#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QMutex>

#define DEBUG 0
#define INFO  1
#define WARN  2
#define CRITI 3
#define FATAL 4
#define NOLOG 5

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    static Logger* getInstance();
    static void outputLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &message);
    quint8 logLevel(QtMsgType type);
    void setLogLevel(quint8 level);

signals:
    void Log(quint8 level, const QString &msg);

private:
    void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &message);
    const QString outputLevel(quint8 level);
    const QString outputFunction(const char *function);

private:
    QMutex  mutex;
    quint8  log_level;
    QString log_dir;
};

#endif // LOGGER_H
