#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void runOperation();

private:
    QLineEdit *txtCommand;
    QPushButton *btnRun;
    QLabel *lblDescription;
};

#endif
