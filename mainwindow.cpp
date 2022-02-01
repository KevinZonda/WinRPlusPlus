#include <QtWidgets>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent),
      cmbCommand(new QComboBox()),
      btnRun(new QPushButton(tr("Run"))),
      btnExit(new QPushButton("Exit")),
      lblDescription(new QLabel(tr("Type the name of a program, folder, document, or Internet resource, and Linux will open it for you."))),
      user(new User())
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Initialise Widgets
    QLabel *lblCmd = new QLabel(tr("&Command:"));
    lblCmd->setBuddy(cmbCommand);
    lblDescription->setWordWrap(true);
    cmbCommand->setEditable(true);
    btnRun->setDefault(true);

    // Initialise ButtonBox
    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(btnRun, QDialogButtonBox::ActionRole);
    buttonBox->addButton(btnExit, QDialogButtonBox::RejectRole);

    // Link Event
    connect(btnRun, &QPushButton::clicked,
            this, &MainWindow::runOperation);
    connect(btnExit, &QPushButton::clicked, this, &MainWindow::close);

    // Set GridLayout
    QGridLayout *mainLayout = new QGridLayout(this);

    // Put Widgets into GridLayout
    mainLayout->addWidget(lblCmd, 1, 0);
    mainLayout->addWidget(cmbCommand, 1, 1);
    mainLayout->addWidget(lblDescription, 0, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    cmbCommand->setFocus();
}

void MainWindow::runOperation()
{
    auto cmd = cmbCommand->currentText();
    user->addHistoryItem(cmd);
    QMessageBox::information(
                this,
                tr("WinR++"),
                cmd);

}
