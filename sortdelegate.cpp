#include "sortdelegate.h"
#include "tablemodel.h"
extern QString bytesToGBMBKB(qint64 size);

SortDelegate::SortDelegate(QObject *parent):QStyledItemDelegate(parent)
{

}

void SortDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    // 进行大小转换
    if (index.column() == FILE_SIZE_COLUMN)
    {
        qint64 bytes = index.data().toLongLong();
        viewOption.text = bytesToGBMBKB(bytes) ;
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &viewOption, painter, viewOption.widget);
    }
    else
    {
        QStyledItemDelegate::paint(painter, viewOption, index);
    }
}
