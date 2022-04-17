#pragma once

#include <QAbstractListModel>
#include <QDateTime>
#include <QStringList>

class Item {
public:
    Item(const QString& value, const QDateTime& timestamp);

    QString value() const;
    QDateTime timestamp() const;

private:
    QString m_value;
    QDateTime m_timestamp;
};

class ItemModel : public QAbstractItemModel {
    Q_OBJECT
public:
    ItemModel(QObject* parent = 0);

    void addValue(const QString& value);
    void removeValue(const QModelIndex& index);

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override
    {
        return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
    }

    QModelIndex sibling(int row, int column, const QModelIndex&) const override
    {
        return index(row, column);
    }

    Qt::ItemFlags flags(const QModelIndex& index) const override
    {
        Qt::ItemFlags f = QAbstractItemModel::flags(index);
        if (index.isValid())
            f |= Qt::ItemNeverHasChildren;
        return f;
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    int columnCount(const QModelIndex& parent) const override
    {
        return parent.isValid() ? 0 : 1;
    }
    bool hasChildren(const QModelIndex& parent) const override
    {
        return parent.isValid() ? false : (rowCount() > 0);
    }
    QModelIndex parent(const QModelIndex& /* index */) const override
    {
        return QModelIndex();
    }

    QList<Item> m_values;
};
