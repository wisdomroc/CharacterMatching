#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H
#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel
{
public:
    SortFilterProxyModel(QObject *parent = nullptr);
    ~SortFilterProxyModel();

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
};

#endif // SORTFILTERPROXYMODEL_H
