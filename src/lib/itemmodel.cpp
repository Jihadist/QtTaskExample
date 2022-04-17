#include "itemmodel.h"
#include "common.h"

Item::Item(const QString& value, const QDateTime& timestamp)
    : m_value(value)
    , m_timestamp(timestamp)
{
}

QString Item::value() const
{
    return m_value;
}

QDateTime Item::timestamp() const
{
    return m_timestamp;
}

ItemModel::ItemModel(QObject* parent)
    : QAbstractItemModel(parent)
{
}

void ItemModel::addValue(const QString& value)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_values.push_back({ value, QDateTime::currentDateTime() });
    endInsertRows();
}

void ItemModel::removeValue(const QModelIndex& index)
{
    if (!index.isValid() || index.row() >= m_values.count())
        return;
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_values.removeAt(index.row());
    endRemoveRows();
}

int ItemModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_values.count();
}

QVariant ItemModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= m_values.count())
        return QVariant();

    const Item& animal = m_values.at(index.row());
    if (role == Qt::DisplayRole)
        return animal.value();
    else if (role == TimeRole)
        return animal.timestamp();
    else if (role == KeyRole)
        return index.row();
    return QVariant();
}

QHash<int, QByteArray> ItemModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TimeRole] = "time";
    roles[KeyRole] = "key";
    return roles;
}
