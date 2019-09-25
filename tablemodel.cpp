#include "tablemodel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

TableModel::~TableModel()
{

}

// 更新表格数据
void TableModel::updateData(QList<FileRecord> recordList)
{
    m_recordList = recordList;
    beginResetModel();
    endResetModel();
    //! Qt4.6以前使用下列代码
    //beginInsertRows(QModelIndex(), 0, m_recordList.count() - 1);
    //endInsertRows();
}

// 行数
int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_recordList.count();
}

// 列数
int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 4;
}

// 设置表格项数据
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int nColumn = index.column();
    FileRecord record = m_recordList.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    {
        if (nColumn == FILE_NAME_COLUMN)
        {
            record.strFileName = value.toString();
        }
        else if (nColumn == DATE_TIME_COLUMN)
        {
            record.dateTime = value.toDateTime();
        }
        // 新增代码
        else if ((nColumn == FILE_SIZE_COLUMN) || (nColumn == FILE_SIZE_HIDDEN_COLUMN))
        {
            record.nSize = value.toLongLong();
        }

        m_recordList.replace(index.row(), record);
        emit dataChanged(index, index);

        // 新增代码
        if ((nColumn == FILE_SIZE_COLUMN) || (nColumn == FILE_SIZE_HIDDEN_COLUMN))
        {
            int nSizeColumn = (nColumn == FILE_SIZE_COLUMN) ? FILE_SIZE_HIDDEN_COLUMN : FILE_SIZE_COLUMN;
            QModelIndex sizeIndex = this->index(index.row(), nSizeColumn);
            emit dataChanged(sizeIndex, sizeIndex);
        }
        return true;
    }
    default:
        return false;
    }
    return false;
}

// 定义GB、MB、KB的计算常量
const int GB = 1024 * 1024 * 1024;
const int MB = 1024 * 1024;
const int KB = 1024;

QString bytesToGBMBKB(qint64 size)
{
    if (size / GB >= 1)
        return QString("%1 GB").arg(QString::number(size / (float)GB, 'f', 2));
    else if (size / MB >= 1)
        return QString("%1 MB").arg(QString::number(size / (float)MB, 'f', 2));
    else if (size / KB >= 1)
        return QString("%1 KB").arg(QString::number(size / (float)KB, 'f', 2));
    else
        return QString("%1 Bytes").arg(size);
}

// 表格项数据
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int nRow = index.row();
    int nColumn = index.column();
    FileRecord record = m_recordList.at(nRow);

    switch (role)
    {
    case Qt::TextColorRole:
        return QColor(Qt::black);
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (nColumn == FILE_NAME_COLUMN)
        {
            return record.strFileName;
        }
        else if (nColumn == DATE_TIME_COLUMN)
        {
            return record.dateTime;
        }
        else if (nColumn == FILE_SIZE_COLUMN)
        {
            return bytesToGBMBKB(record.nSize); //! 改变单位
            return record.nSize;
        }
        // 新增代码
        else if (nColumn == FILE_SIZE_HIDDEN_COLUMN)
        {
            return record.nSize;
        }

        return "";
    }
    case Qt::UserRole:
    {
        // 新增代码
        if (nColumn == FILE_SIZE_COLUMN)
            return record.nSize;
    }
    default:
        return QVariant();
    }

    return QVariant();
}

// 表头数据
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == FILE_NAME_COLUMN)
                return QStringLiteral("名称");

            if (section == DATE_TIME_COLUMN)
                return QStringLiteral("修改日期");

            if (section == FILE_SIZE_COLUMN)
                return QStringLiteral("大小");
        }
    }
    case Qt::UserRole:
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 3)
                return QStringLiteral("大小（字节）");
        }
    }
    default:
        return QVariant();
    }

    return QVariant();
}

// 表格可选中
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return flags;
}
