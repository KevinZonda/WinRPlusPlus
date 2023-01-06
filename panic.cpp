#include "panic.h"
#include <QMessageBox>

void panic(QString title, QString msg) {
    QMessageBox::critical(
                    NULL,
                    title,
                    msg
                );
}

void panic(QString msg) {
    QMessageBox::critical(
                    NULL,
                    "Error",
                    msg
                );
}
