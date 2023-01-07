#include "caller.h"
#include "QtCore/qdebug.h"
#include "panic.h"
#include <QProcess>

#define TERMINAL "zsh"

#ifdef WIN32
    #define TERMINAL "cmd"
#else
    #ifdef Q_OS_MAC
        #define TERMINAL "zsh"
    #else
        #define TERMINAL "bash"
    #endif
#endif

void runWithSystem(QString cmd)
{
    system(cmd.toStdString().c_str());
}

QString getBashEnv() {
    QProcess proc;
    proc.start(TERMINAL, QStringList() << "-i");
    proc.waitForStarted();
    proc.write("echo $PATH\n");
    proc.waitForBytesWritten();
    proc.waitForReadyRead();

    auto path = proc.readAll().trimmed();
    qDebug() << path;
    proc.close();
    return QString(path);
}

void internalCaller(QString cmd)
{
    cmd = cmd.right(cmd.size() - 1);
    if (cmd == "env")
    {
        panic(QString(getenv("PATH")));
    }
    if (cmd == "e") {
        panic(getBashEnv());
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
    int ret = process.startDetached(TERMINAL, QStringList() << QString("-c") << cmd);
    return ret;
}

void runWithBashSync(QString cmd)
{
    if (cmd.startsWith("+"))
    {
        internalCaller(cmd);
        return;
    }
    QProcess process;
    process.setEnvironment(QProcess::systemEnvironment());
    process.start(TERMINAL, QStringList() << QString("-c") << cmd);
    process.waitForFinished();
    if (!process.isReadable()) return;

    auto messager = &info;
    if (process.exitCode() != 0)
        messager = &panic;

    auto out = process.readAll().trimmed();
    auto err = process.readAllStandardError().trimmed();
    QString msg = "";
    if (!out.isEmpty()) {
        msg += "[STDOUT]\n" + out + "\n";
    }

    if (!err.isEmpty()  ) {
        msg += "[STDERR]\n" + err + "\n";
    }

    if (msg.isEmpty()) return;

    (*messager)(msg);
}
