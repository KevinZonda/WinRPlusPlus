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
    QStringList *historyList;
    bool isHistoryUsed;
};

#endif // USER_H
