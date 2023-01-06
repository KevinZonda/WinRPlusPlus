#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <user.h>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QComboBox;
QT_END_NAMESPACE

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QComboBox *cmbCommand;
    User *user;

private slots:
    void runOperation();
    void exitOperation();

private:
    QPushButton *btnRun;
    QPushButton *btnExit;
    QLabel *lblDescription;
};

#endif
