#ifndef SORTDELEGATE_H
#define SORTDELEGATE_H
#include <QStyledItemDelegate>
#include <QApplication>

class SortDelegate : public QStyledItemDelegate
{
public:
    SortDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // SORTDELEGATE_H
