#include "taskmanager.h"

#include <QTimer>
#include <QProcess>
#include <QEventLoop>

#include "config.h"

TaskManager::TaskManager(QObject *parent)
    : QObject{parent}
{

}

TaskManager::~TaskManager()
{

}

bool TaskManager::initEnvironment()
{
    if (!m_envs.isEmpty())
        return true;

#ifdef Q_OS_WINDOWS
    m_envs = QProcessEnvironment::systemEnvironment();
    return true;
#else
    QProcess *proc = nullptr;
    for (const auto sh : TERMINALS)
        if (proc = this->createTask(sh, QStringList() << "-c" << "printenv"); proc)
            break;

    if (!proc)
        return false;

    QEventLoop loop;
    QObject::connect(this, &TaskManager::taskFinished, &loop, &QEventLoop::quit);
    loop.exec();

    QString envsString;
    QVector<QString> envs;
    if (proc->isReadable())
    {
        envsString = proc->readAllStandardOutput();
        envs = envsString.split('\n');
    }
    else
        return true;

    for (const auto &env : qAsConst(envs))
        if (const auto envMap = env.split('='); envMap.size() == 2)
            m_envs.insert(envMap[0], envMap[1]);

    return true;
#endif
}

const QProcessEnvironment &TaskManager::environments() const
{
    return m_envs;
}

QProcess *TaskManager::createTask(const QString &program, const QStringList &arguments)
{
    if (program.isEmpty())
        return nullptr;

    auto proc = new QProcess(this);
    proc->setProcessEnvironment(m_envs);

    proc->start(program, arguments, QProcess::ReadOnly);

    QEventLoop loop;
    QObject::connect(proc, &QProcess::started, &loop, &QEventLoop::quit);
    QTimer::singleShot(1500, &loop, &QEventLoop::quit);
    loop.exec();

    if (proc->state() != QProcess::Running)
    {
        proc->deleteLater();
        return nullptr;
    }

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
