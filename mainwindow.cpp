#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QTextCodec>
#include <QDebug>


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
}


MainWindow::~MainWindow()
{
    delete ui;
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
