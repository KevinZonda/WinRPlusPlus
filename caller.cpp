#include "caller.h"

void runWithSystem(QString cmd) {
    system(cmd.toStdString().c_str());
}
