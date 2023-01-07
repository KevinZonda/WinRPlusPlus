#include "panic.h"
#include <QMessageBox>

void panic(QString title, QString msg) {
    QMessageBox::critical(
                    nullptr,
                    title,
                    msg
                );
}

void panic(QString msg) {
    QMessageBox::critical(
                    nullptr,
                    "Error",
                    msg
                );
}

void info(QString msg) {
    QMessageBox::information(
                    nullptr,
                    "Info",
                    msg
                );
}
