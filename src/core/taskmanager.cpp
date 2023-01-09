#include "taskmanager.h"

#include <QTimer>
#include <QProcess>
#include <QEventLoop>

#ifdef Q_OS_WINDOWS
const char *TERMINAL = "cmd";
#else
#ifdef Q_OS_MAC
const char *TERMINAL = "zsh";
#else
const char *TERMINAL = "bash";
#endif
#endif

TaskManager::TaskManager(QObject *parent)
    : QObject{parent}
{

}

TaskManager::~TaskManager()
{

}

QProcess *TaskManager::createTask(const QString &command)
{
    if (command.isEmpty())
        return nullptr;

    auto proc = new QProcess(this);
    proc->setEnvironment(QProcess::systemEnvironment());

    proc->start(TERMINAL, QStringList() << "-c" << command, QProcess::ReadOnly);

    QEventLoop loop;
    QObject::connect(proc, &QProcess::started, &loop, &QEventLoop::quit);
    QTimer::singleShot(1500, &loop, &QEventLoop::quit);
    loop.exec();

    if (proc->state() != QProcess::Running)
    {
        proc->deleteLater();
        return nullptr;
    }

    proc->closeWriteChannel();

    QObject::connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     this, &TaskManager::onTaskFinished);
    QObject::connect(proc, &QProcess::readyReadStandardOutput,
                     this, &TaskManager::onReadyReadStandardOutput);
    QObject::connect(proc, &QProcess::readyReadStandardError,
                     this, &TaskManager::onReadyReadStandardError);

    ++m_taskCount;

    return proc;
}

void TaskManager::onReadyReadStandardOutput()
{
    emit readyRead(qobject_cast<QProcess *>(sender()), QProcess::StandardOutput);
}

void TaskManager::onReadyReadStandardError()
{
    emit readyRead(qobject_cast<QProcess *>(sender()), QProcess::StandardError);
}

void TaskManager::onTaskFinished(int exitCode, QProcess::ExitStatus)
{
    emit taskFinished(qobject_cast<QProcess *>(sender()), exitCode);

    --m_taskCount;

    if (!m_taskCount)
        emit allTaskFinished();
}
