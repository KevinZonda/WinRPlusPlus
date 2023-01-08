#ifndef RUNNER_H
#define RUNNER_H

#include <QObject>

class QProcess;

class Runner : public QObject
{
    Q_OBJECT
public:
    explicit Runner(QObject *parent = nullptr);

    QProcess *runCommand(const QString &command);

signals:

};

#endif // RUNNER_H
