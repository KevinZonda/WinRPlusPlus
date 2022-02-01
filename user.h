#ifndef USER_H
#define USER_H

#include <QVector>

class User
{
private:
    QVector<QString> *historyList;
    QString* home;
public:
    User();
    void addHistoryItem(QString);
    void saveHistoryItem();

    const QString historyFilePath = ".local/share/WinR/history.txt";
};

#endif // USER_H
