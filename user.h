#ifndef USER_H
#define USER_H

#include <QVector>

class User
{
private:
    QString* home;
public:
    User();
    void addHistoryItem(QString);
    void saveHistoryItem();
    QVector<QString> *historyList;

    const QString historyFilePath = ".local/share/WinR/history.txt";
};

#endif // USER_H
