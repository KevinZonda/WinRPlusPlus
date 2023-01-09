#include "maindialog.h"
#include "ui_maindialog.h"

#include "config.h"
#include "taskmanager.h"

#include <QMessageBox>
#include <QSettings>
#include <QEventLoop>
#include <QRegularExpressionValidator>

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
    , m_manager(new TaskManager(this))
{
    ui->setupUi(this);

    ui->progressBar->hide();

    QObject::connect(ui->runBtn, &QPushButton::clicked, this, &MainDialog::onRun);

    // load history
    m_settings = new QSettings(QSettings::IniFormat, QSettings::Scope::UserScope, "WinRPlusPlus");
    m_settings->setParent(this);

    const auto size = m_settings->beginReadArray("history");
    for (int i = 0; i < size; ++i)
    {
        if (const auto c = m_settings->value("command").toString(); !c.isEmpty())
        {
            m_settings->setArrayIndex(i);
            ui->commandBox->addItem(c);
        }
    }
    m_settings->endArray();

    ui->commandBox->setValidator(new QRegularExpressionValidator(QRegularExpression("\\S.*"), this));
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::onRun()
{
    QString command = ui->commandBox->currentText().trimmed();

    if (command.isEmpty())
        return;

#ifndef Q_OS_WINDOWS
    if (command == "+e")
        command = "echo $PATH";
#endif

    ui->commandBox->insertItem(0, command);

    auto proc = m_manager->createTask(command);

    ui->progressBar->show();

    QEventLoop loop;
    QObject::connect(m_manager, &TaskManager::taskFinished, &loop, &QEventLoop::quit);
    loop.exec();

    ui->progressBar->hide();

    QString output, temp;

    proc->setReadChannel(QProcess::StandardOutput);
    temp = proc->readAll().trimmed();
    if (proc->isReadable() && !temp.isEmpty())
        output.append("[STDOUT]\n").append(temp).append('\n');

    proc->setReadChannel(QProcess::StandardError);
    temp = proc->readAll().trimmed();
    if (proc->isReadable() && !temp.isEmpty())
        output.append("[STDERR]\n").append(proc->readAll().trimmed()).append('\n');

    QMessageBox::information(this, tr("Task Finished"), output);
}

void MainDialog::closeEvent(QCloseEvent *)
{
    // save history
    const auto size = qMax(ui->commandBox->count(), MAX_HISTORY_COUNT);

    m_settings->beginWriteArray("history");
    for (int i = 0; i < size; ++i)
    {
        m_settings->setArrayIndex(i);
        m_settings->setValue("command", ui->commandBox->itemText(i));
    }
    m_settings->endArray();
}
