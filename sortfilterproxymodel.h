#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H
#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    SortFilterProxyModel(QObject *parent = nullptr);
    ~SortFilterProxyModel();

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

#endif // SORTFILTERPROXYMODEL_H
