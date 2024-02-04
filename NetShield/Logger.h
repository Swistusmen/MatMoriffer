#ifndef LOGGER_H
#define LOGGER_H
#include <QQmlContext>
#include <QQmlEngine>
#include <QObject>

class Logger: public QObject
{
    Q_OBJECT
public:
    Logger() {}
    virtual ~Logger() {}

    void addMessage(std::string&&);

public slots:
    void loadLogs();

signals:
    void sentLogMessage(QString);

private:
    bool isApplicationInitialized{false};
    std::vector<std::string> logMessages;
};

#endif // LOGGER_H
