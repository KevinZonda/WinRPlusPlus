#include <QtWidgets>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent),
      txtCommand(new QLineEdit("")),
      btnRun(new QPushButton(tr("Run"))),
      lblDescription(new QLabel(tr("Type the name of a program, folder, document, or Internet resource, and Linux will open it for you.")))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QLabel *lblCmd = new QLabel(tr("&Command:"));
    lblCmd->setBuddy(txtCommand);

    lblDescription->setWordWrap(true);

    btnRun->setDefault(true);
    QPushButton *quitButton = new QPushButton(tr("Quit"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(btnRun, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(btnRun, &QPushButton::clicked,
            this, &MainWindow::runOperation);
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::close);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addWidget(lblCmd, 1, 0);
    mainLayout->addWidget(txtCommand, 1, 1);
    mainLayout->addWidget(lblDescription, 0, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    txtCommand->setFocus();
}

void MainWindow::runOperation()
{
    auto x = txtCommand->text();
    QMessageBox::information(
                this,
                tr("WinR++"),
                x);
}
