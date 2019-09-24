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

    return QSortFilterProxyModel::lessThan(source_left, source_right);
}
