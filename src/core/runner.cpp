#include "runner.h"

#include <QProcess>

#ifdef Q_OS_WINDOWS
const char *TERMINAL = "cmd";
#else
const char *TERMINAL = "sh";
#endif

Runner::Runner(QObject *parent)
    : QObject{parent}
{

}

QProcess *Runner::runCommand(const QString &command)
{
    auto proc = new QProcess(this);
    proc->setEnvironment(QProcess::systemEnvironment());

    proc->start(TERMINAL, QStringList() << "-c" << command);

    return proc;
}
