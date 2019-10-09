#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QTextCodec>
#include <QDebug>
#include <QToolTip>
#include <QTableView>
#include "sortfilterproxymodel.h"
#include "tablemodel.h"
#include "sortdelegate.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //! test information
    TEXT t1;
    t1._text = QString::fromLocal8Bit("湖南");
    t1._firstLetter = firstPinyin( t1._text );
    m_texts.append( t1 );

    TEXT t2;
    t2._text = QString::fromLocal8Bit("河南");
    t2._firstLetter = firstPinyin( t2._text );
    m_texts.append( t2 );

    TEXT t3;
    t3._text = QString::fromLocal8Bit("成都");
    t3._firstLetter = firstPinyin( t3._text );
    m_texts.append( t3 );


    TEXT t4;
    t4._text = QString::fromLocal8Bit("成都123");
    t4._firstLetter = firstPinyin( t4._text );
    m_texts.append( t4 );

    TEXT t5;
    t5._text = QString::fromLocal8Bit("成都456");
    t5._firstLetter = firstPinyin( t5._text );
    m_texts.append( t5 );

    TEXT t6;
    t6._text = QString::fromLocal8Bit("成都789");
    t6._firstLetter = firstPinyin( t6._text );
    m_texts.append( t6 );


    connect( ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()) );
    connect( this, SIGNAL(letterFind(QString)), this, SLOT(onTextChanged(QString)) );

    qDebug() << "master master master" << endl;




    //! 排序功能添加
    QStringList strList;
    strList.append(QString::fromLocal8Bit("天津.卫昆一线"));
    strList.append(QString::fromLocal8Bit("天津.大沽/1#变压器"));
    strList.append(QString::fromLocal8Bit("天津.大沽/2#变压器"));
    strList.append(QString::fromLocal8Bit("天津.春华路/1#变压器"));
    strList.append(QString::fromLocal8Bit("天津.迎丰/1#变压器"));
    strList.append(QString::fromLocal8Bit("天津.华苑/1#变压器"));
    strList.append(QString::fromLocal8Bit("天津.大孟庄/1#变压器"));
    strList.append(QString::fromLocal8Bit("天津.春华路/2#变压器"));
    strList.append(QString::fromLocal8Bit("天津.昆纬路/1#变压器"));
    strList.append(QString::fromLocal8Bit("天津.景顺路/1#变压器"));
    strList.append(QString::fromLocal8Bit("天津.华苑/2#变压器"));
    strList.append(QString::fromLocal8Bit("天津.大孟庄/2#变压器"));
    strList.append(QString::fromLocal8Bit("天津.武清/1#变压器"));
    strList.append(QString::fromLocal8Bit("天津.迎丰/3#变压器"));
    strList.append(QString::fromLocal8Bit("天津.景顺路/2#变压器"));
    strList.append(QString::fromLocal8Bit("天津.华苑/3#变压器"));
    strList.append(QString::fromLocal8Bit("天津.武清/2#变压器"));


    //    qDebug() << "Before:\n" << strList << endl;
    //    qSort(strList.begin(), strList.end());
    //    qDebug() << "After:\n" << strList << endl;





    TableModel *pModel = new TableModel(this);
    SortFilterProxyModel *pProxyModel = new SortFilterProxyModel(this);
    // 设置数据源模型
    pProxyModel->setSourceModel(pModel);
    ui->tableView->setModel(pProxyModel);
    // 设置可排序
    ui->tableView->setSortingEnabled(true);
    // 设置按照文件名升序排列
    ui->tableView->sortByColumn(FILE_NAME_COLUMN, Qt::AscendingOrder);
    // 设置鼠标跟随
    ui->tableView->setMouseTracking(true);
    connect(ui->tableView, SIGNAL(entered(QModelIndex)), this, SLOT(showToolTip(QModelIndex)));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slot_doubleClicked(QModelIndex)));
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->verticalHeader()->setHighlightSections(false);
    // 设置代理
//    ui->tableView->setItemDelegateForColumn(2, new SortDelegate());

    // 构造数据，更新界面
    QList<FileRecord> recordList;
    // 获取随机值
    QTime time = QTime::currentTime();
    qsrand(time.msec() + time.second()*1000);

    for (int i = 0; i < 5; ++i)
    {
        int nIndex = qrand()%20 + 1;
        int nHour = qrand()%24;
        int nMinute = qrand()%60;
        int nSecond = qrand()%60;
        int nBytes = qrand()%100000;

        QDateTime dateTime(QDate(2016, 5, 1), QTime(nHour, nMinute, nSecond));

        FileRecord record;
        record.strFileName = QString("Name %1.cpp").arg(nIndex);
        record.dateTime = dateTime;
        record.nSize = nBytes;
        record.realNum = i;

        recordList.append(record);
    }
    pModel->updateData(recordList);
    m_record = recordList;





    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(recordList.count());
    for(int i = 0; i < recordList.count(); i ++)
    {
        FileRecord record = recordList.at(i);
        QTableWidgetItem *item0 = new QTableWidgetItem(record.strFileName);
        QTableWidgetItem *item1 = new QTableWidgetItem(record.dateTime.toString("yyyyMMdd"));
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(record.nSize));
        ui->tableWidget->setItem(i, 0, item0);
        ui->tableWidget->setItem(i, 1, item1);
        ui->tableWidget->setItem(i, 2, item2);
    }
    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(record_sortbyclounm(int)));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showToolTip(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    int nColumn = index.column();
    if (nColumn == FILE_SIZE_COLUMN)
        QToolTip::showText(QCursor::pos(), index.data().toString());
}

void MainWindow::slot_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    SortFilterProxyModel *proxyModel = qobject_cast<SortFilterProxyModel *>(ui->tableView->model());
    TableModel *tableModel = qobject_cast<TableModel *>(proxyModel->sourceModel());
    QModelIndex realIndex = proxyModel->mapToSource(index);
    tableModel->getData(realIndex);
}

void MainWindow::record_sortbyclounm(int col)
{
    ui->tableWidget->sortItems(col,Qt::AscendingOrder);
}

void MainWindow::onReturnPressed()
{
    QString text = ui->lineEdit->text();
    QString firstLetter = firstPinyin(text);
    qDebug() << "FirstLetter is : " << firstLetter << endl;
    emit letterFind( firstLetter );
}


QString MainWindow::firstPinyin(QString &string)

{
    QTextCodec *codec4gbk = QTextCodec::codecForName("GBK");
    QByteArray buf = codec4gbk->fromUnicode( string );

    int size = buf.size();
    quint16 *array = new quint16[size+1];
    QString firstLetters;
    for( int i = 0, j = 0; i < buf.size(); i++, j++ )
    {
        //gbk的第一个字节都大于0x81，所以小于0x80的都是符号，字母，数字etc
        if( (quint8)buf[i] < 0x80 )
            continue;
        //计算gbk编码
        array[j] = (((quint8)buf[i]) << 8) + (quint8)buf[i+1];
        i++;
        //相当于查表，用gbk编码得到首拼音字母
        firstLetters.append( convert( array[j] ) );

    }

    delete [] array;
    return firstLetters;
}


void MainWindow::onTextChanged(const QString &text)

{
    ui->comboBox->clear();
    int size = m_texts.size();
    int j = 0;
    for( int i = 0; i < size; i ++ )
    {
        QString string = m_texts.at(i)._firstLetter;
        if( string.contains( text ) )
        {
            ui->comboBox->insertItem( j, m_texts.at(i)._text );
        }
    }
}


bool MainWindow::In( wchar_t start, wchar_t end, wchar_t code)

{
    return (code>=start && code<=end) ? true : false;
}

char MainWindow::convert(wchar_t n)
{
    if (In(0xB0A1,0xB0C4,n)) return 'a';
    if (In(0XB0C5,0XB2C0,n)) return 'b';
    if (In(0xB2C1,0xB4ED,n)) return 'c';
    if (In(0xB4EE,0xB6E9,n)) return 'd';
    if (In(0xB6EA,0xB7A1,n)) return 'e';
    if (In(0xB7A2,0xB8c0,n)) return 'f';
    if (In(0xB8C1,0xB9FD,n)) return 'g';
    if (In(0xB9FE,0xBBF6,n)) return 'h';
    if (In(0xBBF7,0xBFA5,n)) return 'j';
    if (In(0xBFA6,0xC0AB,n)) return 'k';
    if (In(0xC0AC,0xC2E7,n)) return 'l';
    if (In(0xC2E8,0xC4C2,n)) return 'm';
    if (In(0xC4C3,0xC5B5,n)) return 'n';
    if (In(0xC5B6,0xC5BD,n)) return 'o';
    if (In(0xC5BE,0xC6D9,n)) return 'p';
    if (In(0xC6DA,0xC8BA,n)) return 'q';
    if (In(0xC8BB,0xC8F5,n)) return 'r';
    if (In(0xC8F6,0xCBF0,n)) return 's';
    if (In(0xCBFA,0xCDD9,n)) return 't';
    if (In(0xCDDA,0xCEF3,n)) return 'w';
    if (In(0xCEF4,0xD188,n)) return 'x';
    if (In(0xD1B9,0xD4D0,n)) return 'y';
    if (In(0xD4D1,0xD7F9,n)) return 'z';
    return '\0';
}
