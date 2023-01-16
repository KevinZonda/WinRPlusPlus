#include "maindialog.h"
#include "ui_maindialog.h"

#include "taskmanager.h"
#include "historymodel.h"

#include <QSettings>
#include <QEventLoop>
#include <QMessageBox>
#include <QScopeGuard>
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

    m_model = new HistoryModel(m_settings, this);
    m_model->load();

    ui->commandBox->setModel(m_model);
    ui->commandBox->setInsertPolicy(QComboBox::NoInsert);
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
    ui->runBtn->setEnabled(false);
    QScopeGuard guard([this] { ui->runBtn->setEnabled(true); });

    const QString c = ui->commandBox->currentText().trimmed();

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

    m_model->addHistory(c);

    ui->progressBar->show();

    QEventLoop loop;
    QObject::connect(m_manager, &TaskManager::taskFinished, &loop, &QEventLoop::quit);
    loop.exec();

    ui->progressBar->hide();

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
    m_model->save();
}

void MainDialog::showEnvironments()
{
    QMessageBox::information(this, tr("System Environments"),
                             m_manager->environments().toStringList().join('\n'));
}
