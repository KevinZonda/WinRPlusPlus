#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QMap>
#include <QObject>
#include <QProcess>

class TaskManager : public QObject
{
    Q_OBJECT    
public:
    explicit TaskManager(QObject *parent = nullptr);
    ~TaskManager() override;

    QProcess *createTask(const QString &command);

    bool isAllFinished() const { return !m_taskCount; }

signals:
    void readyRead(QProcess *sender, QProcess::ProcessChannel readyChannel);
    void taskFinished(QProcess *, int exitCode);
    void allTaskFinished();

private slots:
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onTaskFinished(int exitCode, QProcess::ExitStatus);

private:
    size_t m_taskCount = 0;
};

#endif // TASKMANAGER_H
