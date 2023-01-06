#include "user.h"
#include "helper.cpp"
#include "conv.cpp"
#include "wconst.h"
#include <QStringBuilder>

User::User()
    : historyList(new QVector<QString>())
{
    auto dir = getLinuxHomeDir();

    if (dir == NULL)
    {
        // TODO: when cannot found Linux Home Directory
        //       what to do?
        //       crash? or?
    }
    home = convQStrFromChars(dir);
    auto hist = readAllText(combinePath(*home, historyFilePath));
    auto hists = hist.split("\n");
    foreach (auto item, hists)
    {
        auto trimmed = item.trimmed();
        if (trimmed.isEmpty()) continue;
        historyList->append(item);
    }
}

void User::addHistoryItem(QString cmd)
{
    if (historyList->count() >= MAX_HISTORY_LEN) {
        historyList->removeLast();
    }

    historyList->append(cmd);
    saveHistoryItem();
}

void User::saveHistoryItem()
{
    QStringList ss;
    QVectorIterator<QString> hs(*historyList);

    while (hs.hasNext()){
        ss << hs.next();
    }
}
