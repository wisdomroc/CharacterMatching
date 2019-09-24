#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#include <QAbstractTableModel>
#include <QDateTime>
#include <QColor>


#define FILE_NAME_COLUMN 0   // 文件名
#define DATE_TIME_COLUMN 1   // 修改日期
#define FILE_SIZE_COLUMN 2   // 文件大小

typedef struct FileRecord
{
    QString strFileName; // 文件名
    QDateTime dateTime;  // 修改日期
    qint64 nSize;        // 文件大小
} fileRecord;


class TableModel : public QAbstractTableModel
{
public:
    TableModel(QObject *parent);
    ~TableModel();

    void updateData(QList<FileRecord> recordList);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QList<FileRecord> m_recordList;
};

#endif // TABLEMODEL_H
