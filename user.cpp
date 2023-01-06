#include "user.h"
#include "helper.cpp"
#include "conv.cpp"
#include "wconst.h"
#include <QStringBuilder>

User::User()
    : historyList(new QStringList())
{
    auto dir = getLinuxHomeDir();
    this->isHistoryUsed = true;

    if (dir == NULL)
    {
        panic("Error", "Cannot get $HOME, history list will not be in use!");
        this->isHistoryUsed = false;
        return;
    }
    home = convQStrFromChars(dir);
    ensureDir(combinePath(*home, HISTORY_DIR));
    auto hist = readAllText(combinePath(*home, HISTORY_PATH));
    auto hists = hist.split("\n");
    foreach (auto item, hists)
    {
        auto trimmed = item.trimmed();
        if (trimmed.isEmpty()) continue;
        (*historyList) << trimmed;
    }
}

void User::addHistoryItem(QString cmd)
{
    if (!this->isHistoryUsed) return;

    if (historyList->count() >= MAX_HISTORY_LEN) {
        historyList->removeLast();
    }
    historyList->push_front(cmd);
    saveHistoryItem();
}

void User::saveHistoryItem()
{
    if (!this->isHistoryUsed) return;

    writeAllText(combinePath(*home, HISTORY_PATH), historyList->join("\n"));
}
