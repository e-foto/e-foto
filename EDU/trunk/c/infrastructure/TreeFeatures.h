#ifndef TREEFEATURES_H
#define TREEFEATURES_H

#include <QList>
#include <QVariant>
#include <QAbstractItemModel>
#include <QModelIndex>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class FeatureItem
{
public:
    FeatureItem(const QList<QVariant> &data, FeatureItem *parent = 0);
    ~FeatureItem();

    void appendChild(FeatureItem *child);

    FeatureItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    FeatureItem *parent();

private:
    QList<FeatureItem*> childItems;
    QList<QVariant> itemData;
    FeatureItem *parentItem;
};

class TreeFeatures : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeFeatures(const QString &data, QObject *parent = 0);
    ~TreeFeatures();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    void setupModelData(const QStringList &lines, FeatureItem *parent);

    FeatureItem *rootItem;
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //TREEFEATURES_H
