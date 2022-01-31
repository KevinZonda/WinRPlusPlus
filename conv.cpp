#include <QString>


QString* convQStrFromChars(char* ch)
{
    return new QString(QLatin1String(ch));
}
