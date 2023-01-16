#include "maindialog.h"
#include "ui_maindialog.h"

#include "config.h"
#include "taskmanager.h"

#include <QDebug>
#include <QSettings>
#include <QEventLoop>
#include <QMessageBox>
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

bool MainDialog::init()
{
    if (!m_manager->initEnvironment())
    {
        QMessageBox::critical(this, tr("Oops"), tr("Failed to get system environment variables"));
        return false;
    }

    return true;
}

void MainDialog::onRun()
{
    const QString c = ui->commandBox->currentText().trimmed();

    if (c.isEmpty())
        return;

    if (c == "+e")
    {
        this->showEnvironments();
        return;
    }

    const auto command = c.split(' ');
    auto proc = m_manager->createTask(command.front(), command.mid(1));

    if (!proc)
    {
        QMessageBox::critical(this, tr("Oops"), tr("Unknown command: %1").arg(command.front()));
        return;
    }

    ui->commandBox->insertItem(0, c);

    ui->progressBar->show();
    ui->runBtn->setEnabled(false);

    QEventLoop loop;
    QObject::connect(m_manager, &TaskManager::taskFinished, &loop, &QEventLoop::quit);
    loop.exec();

    ui->progressBar->hide();
    ui->runBtn->setEnabled(true);

    QString output, temp;

    temp = proc->readAllStandardOutput().trimmed();
    if (!temp.isEmpty())
        output.append("[STDOUT]\n").append(temp).append('\n');

    temp = proc->readAllStandardError().trimmed();
    if (!temp.isEmpty())
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

void MainDialog::showEnvironments()
{
    QMessageBox::information(this, tr("System Environments"),
                             m_manager->environments().toStringList().join('\n'));
}
