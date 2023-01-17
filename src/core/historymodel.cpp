#include "historymodel.h"

#include "config.h"

#include <QSettings>

HistoryModel::HistoryModel(QSettings *settings, QObject *parent)
    : QAbstractListModel{parent}
    , m_settings(settings)
{

}

HistoryModel::~HistoryModel()
{

}

void HistoryModel::load()
{
    const auto size = m_settings->beginReadArray("history");
    for (int i = 0; i < size; ++i)
    {
        m_settings->setArrayIndex(i);
        if (const auto c = m_settings->value("command").toString(); !c.isEmpty())
            m_history.append(c);
    }
    m_settings->endArray();
}

void HistoryModel::save()
{
    if (m_history.isEmpty())
        return;

    m_settings->beginWriteArray("history");
    for (int i = 0; i < m_history.size(); ++i)
    {
        m_settings->setArrayIndex(i);
        m_settings->setValue("command", m_history[i]);
    }
    m_settings->endArray();
}

void HistoryModel::addHistory(const QString &history)
{
    if (!m_history.isEmpty() && m_history.first() == history)
        return;

    if (int existIndex = m_history.indexOf(history); existIndex >= 0)
    {
        this->beginMoveRows({}, existIndex, existIndex, {}, 0);
        m_history.move(existIndex, 0);
        this->endMoveRows();
    }
    else
    {
        const auto size = m_history.size();
        if (size == MAX_HISTORY_COUNT)
        {
            this->beginRemoveRows({}, size - 1, size - 1);
            m_history.removeLast();
            this->endRemoveRows();
        }

        this->beginInsertRows({}, 0, 0);
        m_history.prepend(history);
        this->endInsertRows();
    }
}

int HistoryModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_history.size();
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_history.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return m_history.at(index.row());

    return QVariant();
}
