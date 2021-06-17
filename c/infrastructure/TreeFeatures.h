#ifndef TREEFEATURES_H
#define TREEFEATURES_H
/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QList>
#include <QVariant>
#include <QAbstractItemModel>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


class FeatureItem
{
public:
    explicit FeatureItem(const QList<QVariant> &data, FeatureItem *parent = 0);
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
    explicit TreeFeatures(const QString &data, QObject *parent = 0);
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
