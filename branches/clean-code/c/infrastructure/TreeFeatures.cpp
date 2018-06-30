#include "TreeFeatures.h"

#include <QtGui>
#include <QStringList>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

FeatureItem::FeatureItem(const QList<QVariant> &data, FeatureItem *parent)
{
    parentItem = parent;
    itemData = data;
}

FeatureItem::~FeatureItem()
{
    qDeleteAll(childItems);
}

void FeatureItem::appendChild(FeatureItem *item)
{
    childItems.append(item);
}

FeatureItem *FeatureItem::child(int row)
{
    return childItems.value(row);
}

int FeatureItem::childCount() const
{
    return childItems.count();
}

int FeatureItem::columnCount() const
{
    return itemData.count();
}

QVariant FeatureItem::data(int column) const
{
    return itemData.value(column);
}

FeatureItem *FeatureItem::parent()
{
    return parentItem;
}

int FeatureItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<FeatureItem*>(this));

    return 0;
}



TreeFeatures::TreeFeatures(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Features list" << "Details";
    rootItem = new FeatureItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}

TreeFeatures::~TreeFeatures()
{
    delete rootItem;
}

int TreeFeatures::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<FeatureItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant TreeFeatures::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    FeatureItem *item = static_cast<FeatureItem*>(index.internalPointer());

    return item->data(index.column());
}

QModelIndex TreeFeatures::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FeatureItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FeatureItem*>(parent.internalPointer());

    FeatureItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeFeatures::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FeatureItem *childItem = static_cast<FeatureItem*>(index.internalPointer());
    FeatureItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeFeatures::rowCount(const QModelIndex &parent) const
{
    FeatureItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FeatureItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void TreeFeatures::setupModelData(const QStringList &lines, FeatureItem *parent)
{
    QList<FeatureItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != "\t")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new FeatureItem(columnData, parents.last()));
        }

        number++;
    }
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
