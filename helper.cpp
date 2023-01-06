#include "QtCore/qdir.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <QString>
#include <QFile>
#include <QTextStream>

#include <panic.h>
#include <caller.h>

// Get Linux Home Direction
// Attention: this function is not threat-safe
// Refer: https://stackoverflow.com/questions/2910377/get-home-directory-in-linux
char* getLinuxHomeDir()
{
    char *homedir = getenv("HOME");
    if (homedir != NULL)
        return homedir;

    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);

    if (pw == NULL)
        return NULL;

    return pw->pw_dir;
}

QString readAllText(QString path)
{
    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return NULL; // FIXME: maybe need create!
    QTextStream in(&f);
    auto txt = in.readAll();
    f.close();
    return txt;
}

void ensureDir(QString path)
{
    runWithSystem("mkdir -p " + path);
}

void writeAllText(QString path, QString content)
{
    QFile f(path);
    if (!f.open(QIODeviceBase::WriteOnly | QFile::Text))
        return;
    QTextStream out(&f);
    out << content;
    out.flush();
    return;
}

char getPathSep() {
#ifdef WIN32
    return '\\';
#else
    return '/';
#endif
}

QString combinePath(QString path1, QString path2)
{
    if (path1.endsWith(getPathSep())) {
        return path1 + path2;
    }
    return path1 + getPathSep() + path2;
}
