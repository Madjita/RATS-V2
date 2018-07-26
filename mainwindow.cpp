#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    b = new OnixB();
    b->index =1;
    b1 = new OnixB();
    b1->index = 2;

    sss = new Sss();

    auto valid = new QRegExpValidator(QRegExp("^[0-9]*|0"));

    ui->lineEdit->setValidator(valid);
    ui->lineEdit_2->setValidator(valid);
    ui->lineEdit_3->setValidator(valid);
    ui->lineEdit_4->setValidator(valid);
    ui->lineEdit_5->setValidator(valid);

    qRegisterMetaType<QList<quint16>>("QList<quint16>");

    connect(sss,&Sss::writeSSS, b,&OnixB::readSSS);
    connect(b,&OnixB::writeSSS,sss,&Sss::readSSS);

    connect(b1,&OnixB::writeSSS,sss,&Sss::readSSS2);
    connect(sss,&Sss::writeSSS2, b1,&OnixB::readSSS);

    connect(this,&MainWindow::call,b,&OnixB::comandCall);
    connect(this,&MainWindow::call2,b1,&OnixB::comandCall);

    connect(this,&MainWindow::cansel,b,&OnixB::cansel);
    connect(this,&MainWindow::cansel2,b1,&OnixB::cansel);


    connect(b,&OnixB::log,this,&MainWindow::log1);
    connect(b1,&OnixB::log,this,&MainWindow::log1);

    connect(sss,&Sss::log1,this,&MainWindow::log2);
    connect(sss,&Sss::log2,this,&MainWindow::logSSS_2);

    connect(this,&MainWindow::startSimulation,sss,&Sss::startSimulation);
    connect(this,&MainWindow::stopSimulation,sss,&Sss::stopSimulation);

    connect(this,&MainWindow::clearAll,sss,&Sss::clearAll);
    connect(this,&MainWindow::clearAll,b,&OnixB::clearAll);
    connect(this,&MainWindow::clearAll,b1,&OnixB::clearAll);

    connect(b,&OnixB::IS_Gui,ui->radioButton_3,&QRadioButton::setChecked);
    connect(b1,&OnixB::IS_Gui,ui->radioButton_6,&QRadioButton::setChecked);
    connect(b,&OnixB::IS_Gui,ui->radioButton_3,&QRadioButton::clicked);
    connect(b1,&OnixB::IS_Gui,ui->radioButton_6,&QRadioButton::clicked);

    connect(b,&OnixB::Free_Gui,ui->radioButton,&QRadioButton::setChecked);
    connect(b1,&OnixB::Free_Gui,ui->radioButton_4,&QRadioButton::setChecked);
    connect(b,&OnixB::Free_Gui,ui->radioButton,&QRadioButton::clicked);
    connect(b1,&OnixB::Free_Gui,ui->radioButton_4,&QRadioButton::clicked);


    on_lineEdit_4_textChanged(ui->lineEdit_4->text());
    on_lineEdit_3_textChanged(ui->lineEdit_3->text());


//    QDateTime  date = QDateTime::currentDateTime();

   // this->setWindowTitle("Rats [" + date.toLocalTime().toString() + "]");

    sss->setMod_SSS_mod(sss->RECEIPT);

    connect(sss,&Sss::activ,this,&MainWindow::activ);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log1(QString data)
{
    OnixB* obj = (OnixB*)sender(); //Объект который вызвал сигнал

    switch (obj->index)
    {
        case 1:
        {
            ui->textEdit->append(data);
            if(ui->checkBox->checkState() == Qt::Checked)
            {
                 ui->textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
            }

            break;

        }
        case 2:
        {
            ui->textEdit_2->append(data);
            if(ui->checkBox_2->checkState() == Qt::Checked)
            {
                ui->textEdit_2->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
            }
             break;
        }
    }

}

void MainWindow::log2(QString data)
{
    ui->textEdit->append(data);

    if(ui->checkBox->checkState() == Qt::Checked)
        ui->textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::logSSS_2(QString data)
{
    ui->textEdit_2->append(data);
    if(ui->checkBox_2->checkState() == Qt::Checked)
        ui->textEdit_2->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

}

void MainWindow::on_radioButton_2_clicked(bool checked)
{
    if(checked)
    {
        ui->radioButton_5->setEnabled(false);
        ui->radioButton_6->setEnabled(false);

        b->setMod_Onix(b->BUSY);
    }
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    if(checked)
    {
        ui->radioButton_5->setEnabled(true);
        ui->radioButton_6->setEnabled(true);

        b->setMod_Onix(b->FREE);
    }

}

void MainWindow::on_radioButton_3_clicked(bool checked)
{
    if(checked)
    {
        ui->radioButton_5->setEnabled(false);
        ui->radioButton_6->setEnabled(true);

        b->setMod_Onix(b->IS);
    }
}

void MainWindow::on_radioButton_5_clicked(bool checked)
{
    if(checked)
    {
        ui->radioButton_2->setEnabled(false);
        ui->radioButton_3->setEnabled(false);

        b1->setMod_Onix(b1->BUSY);
    }
}

void MainWindow::on_radioButton_4_clicked(bool checked)
{
    if(checked)
    {
        ui->radioButton_2->setEnabled(true);
        ui->radioButton_3->setEnabled(true);

        b1->setMod_Onix(b1->FREE);
    }
}

void MainWindow::on_radioButton_6_clicked(bool checked)
{
    if(checked)
    {
        ui->radioButton_2->setEnabled(false);
        ui->radioButton_3->setEnabled(true);

        b1->setMod_Onix(b1->IS);
    }
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.count() > 0)
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
         ui->pushButton->setEnabled(false);
    }
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(arg1.count() > 0)
    {
        ui->pushButton_3->setEnabled(true);
    }
    else
    {
         ui->pushButton_3->setEnabled(false);
    }
}

void MainWindow::on_pushButton_6_clicked()
{

    ui->pushButton_7->setDefault(false);
    ui->pushButton_8->setDefault(false);

    ui->pushButton_6->setDefault(true);


    if(ui->radioButton_3->isChecked() == true && ui->radioButton_6->isChecked() == true )
    {
        sss->setMod_IS();
    }
    else
    {
        if(ui->radioButton_3->isChecked() == true)
        {
            sss->setMod_IS_1();
        }
        else
        {
            if(ui->radioButton_6->isChecked() == true)
            {
                sss->setMod_IS_2();
            }
        }
    }







    if(ui->lineEdit_5->text().toInt() != 0)
    {
        emit startSimulation(ui->lineEdit_5->text().toInt()*1000);
    }
    else
    {
      ui->lineEdit_5->setText("1");

      emit startSimulation(1000);
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->pushButton_6->setDefault(false);
    ui->pushButton_8->setDefault(false);

    ui->pushButton_7->setDefault(true);

    emit stopSimulation();

    emit clearAll();

    ui->textEdit->clear();
    ui->textEdit_2->clear();


   ui->radioButton->setChecked(true);
   ui->radioButton_4->setChecked(true);
   ui->radioButton_7->setChecked(true);
   ui->radioButton_7->clicked(true);


   ui->radioButton_2->setEnabled(true);
   ui->radioButton_3->setEnabled(true);

   ui->radioButton_5->setEnabled(true);
   ui->radioButton_6->setEnabled(true);

}

void MainWindow::on_pushButton_8_clicked()
{
    ui->pushButton_6->setDefault(false);
    ui->pushButton_7->setDefault(false);

    ui->pushButton_8->setDefault(true);

    emit stopSimulation();
}


void MainWindow::Number_1(QList<quint16>& number_1, QByteArray text,int i)
{
    switch (text[i]) {
            case '0':
            {
                number_1.append(quint16(b->ZERO_1));
                break;
            }
            case '1':
            {
                number_1.append(quint16(b->ONE_1));
                break;
            }
            case '2':
            {
                number_1.append(quint16(b->TWO_1));
                break;
            }
            case '3':
            {
                number_1.append(quint16(b->THREE_1));
                break;
            }
            case '4':
            {
                number_1.append(quint16(b->FOUR_1));
                break;
            }
            case '5':
            {
                number_1.append(quint16(b->FIVE_1));
                break;
            }
            case '6':
            {
                number_1.append(quint16(b->SIX_1));
                break;
            }
            case '7':
            {
                number_1.append(quint16(b->SEVEN_1));
                break;
            }
            case '8':
            {
                number_1.append(quint16(b->EIGHT_1));
                break;
            }
            case '9':
            {
                number_1.append(quint16(b->NINE_1));
                break;
            }
    }
}

void MainWindow::Number_2(QList<quint16> &number_1, QByteArray text, int i)
{
    switch (text[i]) {
            case '0':
            {
                number_1.append(quint16(b->ZERO_2));
                break;
            }
            case '1':
            {
                number_1.append(quint16(b->ONE_2));
                break;
            }
            case '2':
            {
                number_1.append(quint16(b->TWO_2));
                break;
            }
            case '3':
            {
                number_1.append(quint16(b->THREE_2));
                break;
            }
            case '4':
            {
                number_1.append(quint16(b->FOUR_2));
                break;
            }
            case '5':
            {
                number_1.append(quint16(b->FIVE_2));
                break;
            }
            case '6':
            {
                number_1.append(quint16(b->SIX_2));
                break;
            }
            case '7':
            {
                number_1.append(quint16(b->SEVEN_2));
                break;
            }
            case '8':
            {
                number_1.append(quint16(b->EIGHT_2));
                break;
            }
            case '9':
            {
                number_1.append(quint16(b->NINE_2));
                break;
            }
    }
}

void MainWindow::Number_3(QList<quint16> &number_1, QByteArray text, int i)
{
    switch (text[i]) {
            case '0':
            {
                number_1.append(quint16(b->ZERO_3));
                break;
            }
            case '1':
            {
                number_1.append(quint16(b->ONE_3));
                break;
            }
            case '2':
            {
                number_1.append(quint16(b->TWO_3));
                break;
            }
            case '3':
            {
                number_1.append(quint16(b->THREE_3));
                break;
            }
            case '4':
            {
                number_1.append(quint16(b->FOUR_3));
                break;
            }
            case '5':
            {
                number_1.append(quint16(b->FIVE_3));
                break;
            }
            case '6':
            {
                number_1.append(quint16(b->SIX_3));
                break;
            }
            case '7':
            {
                number_1.append(quint16(b->SEVEN_3));
                break;
            }
            case '8':
            {
                number_1.append(quint16(b->EIGHT_3));
                break;
            }
            case '9':
            {
                number_1.append(quint16(b->NINE_3));
                break;
            }
    }
}

void MainWindow::Number_4(QList<quint16> &number_1, QByteArray text, int i)
{
    switch (text[i]) {
            case '0':
            {
                number_1.append(quint16(b->ZERO_4));
                break;
            }
            case '1':
            {
                number_1.append(quint16(b->ONE_4));
                break;
            }
            case '2':
            {
                number_1.append(quint16(b->TWO_4));
                break;
            }
            case '3':
            {
                number_1.append(quint16(b->THREE_4));
                break;
            }
            case '4':
            {
                number_1.append(quint16(b->FOUR_4));
                break;
            }
            case '5':
            {
                number_1.append(quint16(b->FIVE_4));
                break;
            }
            case '6':
            {
                number_1.append(quint16(b->SIX_4));
                break;
            }
            case '7':
            {
                number_1.append(quint16(b->SEVEN_4));
                break;
            }
            case '8':
            {
                number_1.append(quint16(b->EIGHT_4));
                break;
            }
            case '9':
            {
                number_1.append(quint16(b->NINE_4));
                break;
            }
    }
}

void MainWindow::activ()
{
    if(ui->pushButton->isEnabled() == false && ui->lineEdit->text().count() > 2)
    {
       ui->pushButton->setEnabled(true);
    }

    if(ui->pushButton_3->isEnabled() == false && ui->lineEdit_2->text().count() > 2)
    {
       ui->pushButton_3->setEnabled(true);
    }

    if(ui->pushButton_2->isEnabled() == false )
    {
       ui->pushButton_2->setEnabled(true);
    }

    if(ui->pushButton_4->isEnabled() == false)
    {
       ui->pushButton_4->setEnabled(true);
    }

}

void MainWindow::on_pushButton_3_clicked()
{

    QByteArray text = ui->lineEdit_2->text().toUtf8();

    QList<quint16> number_1;
    QList<quint16> number_2;

    ui->pushButton_3->setEnabled(false);


    for(int i=0; i < text.count();i++)
    {



        if(i < 4)
        {
            switch (i) {
                case 0:
                {
                    Number_1(number_1,text,i);
                    break;
                }
                case 1:
                {
                    Number_2(number_1,text,i);
                    break;
                }
                case 2:
                {
                    Number_3(number_1,text,i);
                    break;
                }
                case 3:
                {
                    Number_4(number_1,text,i);
                    break;
                }
            }
        }
        else
        {

            switch (i) {
                case 4:
                {
                    Number_1(number_2,text,i);
                    break;
                }
                case 5:
                {
                    Number_2(number_2,text,i);
                    break;
                }
                case 6:
                {
                    Number_3(number_2,text,i);
                    break;
                }
                case 7:
                {
                    Number_4(number_2,text,i);
                    break;
                }
            }
        }


    }


    qDebug () << number_1;

     qDebug () << number_2;


    emit call2(number_1,number_2);
}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{

    QByteArray text = ui->lineEdit_3->text().toUtf8();

    QList<quint16> number_1;
    QList<quint16> number_2;


    for(int i=0; i < text.count();i++)
    {



        if(i < 4)
        {
            switch (i) {
                case 0:
                {
                    Number_1(number_1,text,i);
                    break;
                }
                case 1:
                {
                    Number_2(number_1,text,i);
                    break;
                }
                case 2:
                {
                    Number_3(number_1,text,i);
                    break;
                }
                case 3:
                {
                    Number_4(number_1,text,i);
                    break;
                }
            }
        }
        else
        {

            switch (i) {
                case 4:
                {
                    Number_1(number_2,text,i);
                    break;
                }
                case 5:
                {
                    Number_2(number_2,text,i);
                    break;
                }
                case 6:
                {
                    Number_3(number_2,text,i);
                    break;
                }
                case 7:
                {
                    Number_4(number_2,text,i);
                    break;
                }
            }
        }


    }


    b->setNumber_1(number_1,number_2);
}

void MainWindow::on_lineEdit_4_textChanged(const QString &arg1)
{
    QByteArray text = ui->lineEdit_4->text().toUtf8();

    QList<quint16> number_1;
    QList<quint16> number_2;


    for(int i=0; i < text.count();i++)
    {



        if(i < 4)
        {
            switch (i) {
                case 0:
                {
                    Number_1(number_1,text,i);
                    break;
                }
                case 1:
                {
                    Number_2(number_1,text,i);
                    break;
                }
                case 2:
                {
                    Number_3(number_1,text,i);
                    break;
                }
                case 3:
                {
                    Number_4(number_1,text,i);
                    break;
                }
            }
        }
        else
        {

            switch (i) {
                case 4:
                {
                    Number_1(number_2,text,i);
                    break;
                }
                case 5:
                {
                    Number_2(number_2,text,i);
                    break;
                }
                case 6:
                {
                    Number_3(number_2,text,i);
                    break;
                }
                case 7:
                {
                    Number_4(number_2,text,i);
                    break;
                }
            }
        }


    }


    b1->setNumber_1(number_1,number_2);
}

void MainWindow::on_pushButton_clicked()
{
    QByteArray text = ui->lineEdit->text().toUtf8();

    QList<quint16> number_1;
    QList<quint16> number_2;


    ui->pushButton->setEnabled(false);


    for(int i=0; i < text.count();i++)
    {



        if(i < 4)
        {
            switch (i) {
                case 0:
                {
                    Number_1(number_1,text,i);
                    break;
                }
                case 1:
                {
                    Number_2(number_1,text,i);
                    break;
                }
                case 2:
                {
                    Number_3(number_1,text,i);
                    break;
                }
                case 3:
                {
                    Number_4(number_1,text,i);
                    break;
                }
            }
        }
        else
        {

            switch (i) {
                case 4:
                {
                    Number_1(number_2,text,i);
                    break;
                }
                case 5:
                {
                    Number_2(number_2,text,i);
                    break;
                }
                case 6:
                {
                    Number_3(number_2,text,i);
                    break;
                }
                case 7:
                {
                    Number_4(number_2,text,i);
                    break;
                }
            }
        }


    }


    qDebug () << number_1;

     qDebug () << number_2;


    emit call(number_1,number_2);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton_2->setEnabled(false);

    emit cansel();
}

void MainWindow::on_pushButton_4_clicked()
{
   ui->pushButton_4->setEnabled(false);

   emit cansel2();
}


void MainWindow::on_radioButton_7_clicked(bool checked)
{
    if(checked)
    {
        sss->setMod_SSS_fz(NULL);
        sss->setMod_SSS(sss->FREE);
    }
}

void MainWindow::on_radioButton_10_clicked(bool checked)
{
    if(checked)
    {
        sss->setMod_SSS_fz(NULL);
        sss->setMod_SSS(sss->BUSY);
    }
}

void MainWindow::on_radioButton_13_clicked(bool checked)
{
    if(checked)
    {
        sss->setMod_SSS_fz(NULL);
        sss->setMod_SSS(sss->HANGUP);

        if(ui->radioButton_15->isChecked())
        {
            sss->setMod_SSS_fz(sss->HANGUP);
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    sss->setMod_SSS(sss->PRIORITY_CALL_OFF);
}

void MainWindow::on_radioButton_8_clicked(bool checked)
{
    if(checked)
    {
        sss->setMod_SSS_fz(NULL);
        sss->setMod_SSS(sss->BUSY_PHASING);

        qDebug () << ui->radioButton_3->isChecked();

        if( ui->radioButton_3->isChecked() == true)
        {
             return;
        }
        else
        {
            qDebug () << ui->radioButton_6->isChecked();

            if( ui->radioButton_6->isChecked() == true)
            {
                 return;
            }
            else
            {
                // sss->setMod_SSS_fz(sss->BUSY_PHASING);
            }
        }



    }
}

void MainWindow::on_radioButton_12_clicked(bool checked)
{
    if(checked)
    {
        sss->setMod_SSS_fz(NULL);
        sss->setMod_SSS(sss->WORK);
    }
}

void MainWindow::on_radioButton_15_clicked(bool checked)
{
    if(checked)
    {
        sss->setMod_SSS_mod(sss->PHASING);
    }
}

void MainWindow::on_radioButton_14_clicked(bool checked)
{
    if(checked)
    {
        sss->setMod_SSS_mod(sss->RECEIPT);
    }
}

void MainWindow::on_radioButton_9_clicked(bool checked)
{
    if(checked)
    {
        sss->setMod_SSS_fz(sss->BUSY_PHASING);
        sss->setMod_SSS(sss->BUSY_PHASING);
    }
}

void MainWindow::on_radioButton_11_clicked(bool checked)
{
    if(checked)
    {
        sss->setMod_SSS_fz(sss->BUSY);
        sss->setMod_SSS(sss->BUSY);
    }
}
