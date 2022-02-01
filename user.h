#ifndef USER_H
#define USER_H

#include <QVector>

class user
{
private:
    QVector<QString> *historyList;
    QString* home;
public:
    user();

    const QString historyFilePath = ".local/share/WinR/history.txt";
};

#endif // USER_H
