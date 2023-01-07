#include "caller.h"
#include "panic.h"
#include <QProcess>

void runWithSystem(QString cmd)
{
    system(cmd.toStdString().c_str());
}

void internalCaller(QString cmd)
{
    cmd = cmd.right(cmd.size() - 1);
    if (cmd == "env")
    {
        panic(QString(getenv("PATH")));
    }

}

int runWithBash(QString cmd)
{
    if (cmd.startsWith("+"))
    {
        internalCaller(cmd);
        return 0;
    }
    QProcess process;
    process.setEnvironment(QProcess::systemEnvironment());
#ifdef WIN32
    return process.startDetached("cmd", QStringList() << QString("/c") << cmd);
#else
    return process.startDetached("bash", QStringList() << QString("-c") << cmd);
#endif
}
