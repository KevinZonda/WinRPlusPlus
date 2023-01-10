#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class QSettings;

class TaskManager;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

    bool init();

public slots:
    void onRun();

private:
    void closeEvent(QCloseEvent *) override;

    void showEnvironments();

    Ui::MainDialog *ui;

    QSettings *m_settings = nullptr;

    TaskManager *m_manager = nullptr;
};
#endif // MAINDIALOG_H
