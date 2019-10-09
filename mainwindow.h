#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tablemodel.h"

namespace Ui {
 class MainWindow;
}

struct TEXT
{
 QString _text;
 QString _firstLetter;
};

class MainWindow : public QMainWindow
{
 Q_OBJECT

public:
 explicit MainWindow(QWidget *parent = nullptr);
 ~MainWindow();

private:
 Ui::MainWindow *ui;
 QList<TEXT> m_texts;

signals:
 void letterFind(const QString &text);

private slots:
 void onTextChanged(const QString &text);
 void onReturnPressed();
 void record_sortbyclounm(int col);
 void showToolTip(const QModelIndex &index);
 void slot_doubleClicked(const QModelIndex &index);
private:
 QString firstPinyin(QString &text);
 bool In(wchar_t start, wchar_t end, wchar_t code);
 char convert(wchar_t n);//! 用GBK码查找到汉字的首拼音字母

 QList<FileRecord> m_record;
};
#endif // MAINWINDOW_H
