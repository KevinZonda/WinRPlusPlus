#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QStringList>
#include <QAbstractListModel>

class QSettings;

class HistoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit HistoryModel(QSettings *settings, QObject *parent = nullptr);
    ~HistoryModel() override;

    void load();
    void save();

    void addHistory(const QString &history);

    int rowCount(const QModelIndex &) const override;

    QVariant data(const QModelIndex &, int) const override;

signals:

private:
    QSettings *m_settings = nullptr;
    QStringList m_history;
};

#endif // HISTORYMODEL_H
