#include "user.h"
#include "helper.cpp"
#include "conv.cpp"

user::user()
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
}
