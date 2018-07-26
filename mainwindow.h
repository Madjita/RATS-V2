#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>

#include <onixb.h>
#include <sss.h>

#include <QRegExpValidator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    OnixB* b;
    OnixB* b1;

    Sss* sss;


    void Number_1(QList<quint16>& number_1, QByteArray text,int i);
    void Number_2(QList<quint16>& number_1, QByteArray text,int i);
    void Number_3(QList<quint16>& number_1, QByteArray text,int i);
    void Number_4(QList<quint16>& number_1, QByteArray text,int i);

signals:
    void startSimulation(int);
    void stopSimulation();
    void clearAll();

    void call(QList<quint16> number_1,QList<quint16> number_2);
    void call2(QList<quint16> number_1,QList<quint16> number_2);

    void cansel();
    void cansel2();


private slots:
    void activ();
    void log1(QString data);
    void log2(QString data);
    void logSSS_2(QString data);

    void on_radioButton_2_clicked(bool checked);

    void on_radioButton_clicked(bool checked);

    void on_radioButton_3_clicked(bool checked);

    void on_radioButton_5_clicked(bool checked);

    void on_radioButton_4_clicked(bool checked);

    void on_radioButton_6_clicked(bool checked);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);


    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_3_clicked();

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();


    void on_radioButton_7_clicked(bool checked);

    void on_radioButton_10_clicked(bool checked);

    void on_radioButton_13_clicked(bool checked);

    void on_pushButton_5_clicked();

    void on_radioButton_8_clicked(bool checked);

    void on_radioButton_12_clicked(bool checked);

    void on_radioButton_15_clicked(bool checked);

    void on_radioButton_14_clicked(bool checked);

    void on_radioButton_9_clicked(bool checked);

    void on_radioButton_11_clicked(bool checked);

private:
    Ui::MainWindow *ui;

//   typedef struct {
//        quint32 a;
//        quint8 b;
//    } quint40;

};

#endif // MAINWINDOW_H
