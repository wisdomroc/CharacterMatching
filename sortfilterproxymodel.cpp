#include "sortfilterproxymodel.h"
#include "tablemodel.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

SortFilterProxyModel::~SortFilterProxyModel()
{

}

bool SortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if (!source_left.isValid() || !source_right.isValid())
        return false;

    if ((source_left.column() == FILE_NAME_COLUMN) && (source_right.column() == FILE_NAME_COLUMN))
    {
        QVariant leftData = sourceModel()->data(source_left);
        QVariant rightData = sourceModel()->data(source_right);

        if (leftData.canConvert<QString>() && rightData.canConvert<QString>())
        {
            QString strLeft = leftData.toString();
            QString strRight = rightData.toString();

            // 去掉后缀.cpp
            if (strLeft.contains("."))
            {
                int nIndex = strLeft.lastIndexOf(".");
                strLeft = strLeft.left(nIndex);
            }
            if (strRight.contains("."))
            {
                int nIndex = strRight.lastIndexOf(".");
                strRight = strRight.left(nIndex);
            }

            // 比较大小，如果字符串相同，则比较后面的整形数据
            QStringList strLeftList = strLeft.split(" ");
            QStringList strRightList = strRight.split(" ");
            if ((strLeftList.count() >= 2) && (strRightList.count() >= 2))
            {
                int nResult = QString::compare(strLeftList.at(0), strRightList.at(0), Qt::CaseInsensitive);
                if (nResult == 0)
                {
                    return strLeftList.at(1).toInt() < strRightList.at(1).toInt();
                }
                else
                {
                    return nResult;
                }
            }
        }
    }
    //! 这里是加了辅助排序列以后的代码
    if ((source_left.column() == FILE_SIZE_COLUMN) && (source_right.column() == FILE_SIZE_COLUMN))
    {
        // 获取辅助列索引
        QModelIndex sizeLeftIndex = sourceModel()->index(source_left.row(), FILE_SIZE_HIDDEN_COLUMN);
        QModelIndex sizeRightIndex = sourceModel()->index(source_right.row(), FILE_SIZE_HIDDEN_COLUMN);

        QVariant leftData = sourceModel()->data(sizeLeftIndex);
        QVariant rightData = sourceModel()->data(sizeRightIndex);

        if (leftData.canConvert<qint64>() && rightData.canConvert<qint64>())
        {
            return leftData.toLongLong() < rightData.toLongLong();
        }
    }
    //! 如果没有辅助排序列，则用一下代码
    if ((source_left.column() == FILE_SIZE_COLUMN) && (source_right.column() == FILE_SIZE_COLUMN))
    {
        // 这里我们所取得数据是用户源数据
        QVariant leftData = sourceModel()->data(source_left, Qt::UserRole);
        QVariant rightData = sourceModel()->data(source_right, Qt::UserRole);

        if (leftData.canConvert<qint64>() && rightData.canConvert<qint64>())
        {
            return leftData.toLongLong() < rightData.toLongLong();
        }
    }

    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

QVariant SortFilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        return section + 1;
    }
    else
    {
        return QSortFilterProxyModel::headerData(section, orientation, role);
    }
}
