#include "maindialog.h"
#include "ui_maindialog.h"

#include <QSettings>
#include <QRegularExpressionValidator>

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->runBtn, &QPushButton::clicked, this, &MainDialog::onRun);

    // load history
    m_settings = new QSettings(QSettings::Scope::UserScope, "WinRPlusPlus");

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
    const QString command = ui->commandBox->currentText().trimmed();

    if (command.isEmpty())
        return;

    ui->commandBox->insertItem(0, command);


}

void MainDialog::closeEvent(QCloseEvent *)
{
    // save history
    const auto size = ui->commandBox->count();

    m_settings->beginWriteArray("history");
    for (int i = 0; i < size; ++i)
    {
        m_settings->setArrayIndex(i);
        m_settings->setValue("command", ui->commandBox->itemText(i));
    }
    m_settings->endArray();
}
