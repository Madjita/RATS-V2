#include "onixb.h"

OnixB::OnixB(QObject *parent) : QObject(parent),
    Mod_Onix(quint16(FREE)),
    Mod_SSS(quint16(FREE))
{
    this->moveToThread(new QThread());

    connect(this->thread(),&QThread::started,this,&OnixB::process_start);

    this->thread()->start();
}

void OnixB::process_start()
{
    clearAll();
}

void OnixB::readSSS(quint16 data)
{

    if(setNumber == false)
    {
        switch (data) {

        case PHASING:
        {
            if(Mod_Onix == CALL && Mod_SSS == FREE)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ФАЗИРВОАНИЕ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду ФАЗИРВОАНИЕ.";

                Mod_SSS = PHASING;
                Mod_Onix = FREE;

                writeSSS(FREE);

                return;
            }

            break;
        }

        case CALL2Replay:
        {
            if(Mod_Onix == PPV && Mod_SSS == CALL2)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ППО</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>ИС</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду ППО.";

                Mod_SSS = IS;
                Mod_Onix = IS;

                writeSSS(IS);


                return;
            }

            break;
        }

        case CALL2: //(PPV)
        {
            if(Mod_Onix == RADIO && Mod_SSS == WORK)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ВЫЗОВ2</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Вызов2.";

                Mod_Onix == RADIO;
                Mod_SSS = CALL2;

                Number_1.clear();
                setNumber = true;

                return;
            }


            if(Mod_Onix == CALL2 && Mod_SSS == RADIO)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ППВ</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду ППВ.";

                Mod_SSS = PPV;

                Number_1.clear();
                setNumber = true;

                return;
            }

            break;
        }

        case RADIO:
        {
            if(Mod_Onix == FREE && Mod_SSS == RECEIPT)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>РАДИО</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>ВЫЗОВ2</b>. Номер: <b>"+ nomer(Number_2) +"</b>\n");

                qDebug () << index << ") OnixB принял от SSS команду РАДИО.";
                qDebug () << index << ") OnixB отправил  команду ВЫЗОВ2.";


                Mod_Onix = CALL2;
                Mod_SSS = RADIO;

                emit writeSSS(CALL2);

                emit writeSSS(Number_2.value(0));
                emit writeSSS(Number_2.value(1));
                emit writeSSS(Number_2.value(2));
                emit writeSSS(Number_2.value(3));

                return;
            }
            break;
        }

        case BUSY_PHASING:
        {

            if(Mod_Onix == CALL && Mod_SSS == FREE)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";


                Mod_Onix = FREE;
                Mod_SSS = BUSY_PHASING;

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == RECEIPT)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";


                Mod_Onix = FREE;
                Mod_SSS = BUSY_PHASING;

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == PHASING)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";


                Mod_Onix = FREE;
                Mod_SSS = BUSY_PHASING;

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == CALL && Mod_SSS == BUSY_PHASING)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";


                Mod_Onix = FREE;
                Mod_SSS = BUSY_PHASING;

                emit writeSSS(FREE);
                return;
            }

            break;
        }

        case PRIORITY_CALL_OFF:
        {
            if(Mod_Onix == IS && Mod_SSS == IS)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ ПО ПРИОРИТЕТУ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду ОТБОЙ ПО ПРИОРИТЕТУ.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";


                Mod_Onix = FREE;
                Mod_SSS = PRIORITY_CALL_OFF;

                emit writeSSS(FREE);
                return;
            }
            break;
        }

        case BUSY:
        {
            if(Mod_Onix == FREE && Mod_SSS == RECEIPT)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ЗАНЯТО</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Свободен.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";


                Mod_Onix = FREE;
                Mod_SSS = FREE;

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == IS && Mod_SSS == IS)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ЗАНЯТО</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Свободен.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";


                Mod_Onix = FREE;
                Mod_SSS = BUSY;

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == CALL && Mod_SSS == FREE)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ЗАНЯТО</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Свободен.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";


                Mod_Onix = FREE;
                Mod_SSS = BUSY;

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == PHASING)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ЗАНЯТО</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Свободен.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";


                Mod_Onix = FREE;
                Mod_SSS = BUSY;

                emit writeSSS(FREE);
                return;
            }

            break;
        }
        case FREE:
        {

            if(Mod_Onix == FREE && Mod_SSS == FREE)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДЕН</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Свободен.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == WORK && Mod_SSS == WORK)
            {

                if(DefaultNumber_2.value(0) == ZERO_1 && DefaultNumber_2.value(1) == REPEAT_1 && DefaultNumber_2.value(2) == ZERO_3 && DefaultNumber_2.value(3) == REPEAT_3)
                {

                }
                else
                {
                    emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДЕН</b>.\n");

                    timerCount = 0;

                    emit log(QString::number(index) + ") OnixB отправил  команду <b>РАДИО</b>.\n");

                    Mod_Onix = RADIO ;
                    Mod_SSS = CALL;

                    emit writeSSS(RADIO); // РАДИО
                    return;
                }

                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДЕН</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>РАБОТА</b>.\n" + QString::number(Number_1.value(0),16) +"\n" + QString::number(Number_1.value(1),16) +"\n" + QString::number(Number_1.value(2),16)+"\n" + QString::number(Number_1.value(3),16) +"\n");

                // qDebug () << index << ") OnixB принял от SSS команду СВОБОДЕН.";
                // qDebug () << index << ") OnixB отправил  команду СВОБОДЕН.";

                emit writeSSS(WORK); // РАБОТА

                emit writeSSS(Number_1.value(0));
                emit writeSSS(Number_1.value(1));
                emit writeSSS(Number_1.value(2));
                emit writeSSS(Number_1.value(3));

                timerCount++;

                if(timerCount >5)
                {
                    timerCount = 0;

                    Mod_Onix = IS ;
                    Mod_SSS = IS;

                    emit IS_Gui(true);

                    emit writeSSS(IS); // РАБОТА
                }
                else
                {
                    emit log("Таймер : " + QString::number(timerCount) + " c");

                }


                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == HANGUP)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДЕН</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = FREE;

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == BUSY && Mod_SSS == CALL)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДЕН</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = FREE;
                Mod_Onix = FREE;

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == BUSY)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДЕН</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = FREE;
                Mod_Onix = FREE;


                emit Free_Gui(true);

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == PRIORITY_CALL_OFF)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДЕН</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = FREE;
                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == BUSY_PHASING)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДЕН</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = FREE;
                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == RECEIPT)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДЕН</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = FREE;
                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == CALL)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДЕН</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = FREE;
                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit writeSSS(FREE);
                return;
            }





            break;

        }

        case RECEIPT:
        {
            if(Mod_Onix == FREE && Mod_SSS == FREE)
            {
                emit log(QString::number(index) + ") OnixB принял SSS команду <b>КВИТАНЦИЯ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял SSS команду КВИТАНЦИЯ.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";

                Mod_SSS = RECEIPT;
                Mod_Onix = FREE;


                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == CALL && Mod_SSS == FREE)
            {
                emit log(QString::number(index) + ") OnixB принял SSS команду <b>КВИТАНЦИЯ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял SSS команду Свободен.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";

                Mod_SSS = RECEIPT;
                Mod_Onix = FREE;


                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == RECEIPT)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>КВИТАНЦИЯ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Свободен.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";

                emit writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == PHASING)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>КВИТАНЦИЯ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Свободен.";
                qDebug () << index << ") OnixB отправил  команду Свободен.";

                Mod_SSS = RECEIPT;

                emit writeSSS(FREE);
                return;
            }

            break;
        }

        case CALL:
        {

            if(Mod_Onix == CALL && Mod_SSS == FREE)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>РАБОТА</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Работа.";

                Number_1.clear();
                setNumber = true;
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == FREE)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ВЫЗОВ</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Вызов.";

                Mod_Onix = FREE;
                Mod_SSS = CALL;

                Number_1.clear();
                setNumber = true;

                return;
            }

            if(Mod_Onix == FREE  && Mod_SSS == RECEIPT )
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>РАБОТА</b>.\n");

                qDebug () << index << ") OnixB принял от SSS команду Работа.";

                Number_1.clear();
                setNumber = true;
            }


            break;
        }

        case IS:
        {
            emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ИС</b>.\n");
            emit log(QString::number(index) + ") OnixB отправил  команду <b>ИС</b>.\n");
            Mod_Onix = IS;
            Mod_SSS = IS;

            emit  writeSSS(IS);

            return;

            break;
        }

        case HANGUP:
        {



            if(Mod_Onix == HANGUP && Mod_SSS == RECEIPT)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }

            if(Mod_Onix == CALL && Mod_SSS == CALL)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }

            if(Mod_Onix == HANGUP && Mod_SSS == HANGUP)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == RECEIPT)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = HANGUP;
                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }

            if(Mod_Onix == CALL && Mod_SSS == FREE)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = HANGUP;
                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }

            if(Mod_Onix == HANGUP && Mod_SSS == IS)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = HANGUP;
                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit  writeSSS(FREE);
                return;
            }

            if(Mod_Onix == IS && Mod_SSS == IS)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = HANGUP;
                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit  writeSSS(FREE);
                return;
            }

            if(Mod_Onix == FREE && Mod_SSS == PHASING)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_SSS = HANGUP;
                Mod_Onix = FREE;

                emit Free_Gui(true);

                emit  writeSSS(FREE);
                return;
            }

            if(Mod_Onix == HANGUP && Mod_SSS == PHASING)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_Onix = FREE;
                Mod_SSS = HANGUP;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }

            if(Mod_Onix == CALL2 && Mod_SSS == RADIO)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_Onix = FREE;
                Mod_SSS = FREE;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }

            if(Mod_Onix == RADIO && Mod_SSS == CALL)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_Onix = FREE;
                Mod_SSS = HANGUP;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }

            if(Mod_Onix == HANGUP && Mod_SSS == RADIO)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_Onix = FREE;
                Mod_SSS = HANGUP;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }

            if(Mod_Onix == CALL2 && Mod_SSS == CALL2)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_Onix = FREE;
                Mod_SSS = HANGUP;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }

            if(Mod_Onix == HANGUP && Mod_SSS == CALL2)
            {
                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>.\n");
                emit log(QString::number(index) + ") OnixB отправил  команду <b>СВОБОДЕН</b>.\n");

                Mod_Onix = FREE;
                Mod_SSS = HANGUP;

                emit Free_Gui(true);

                emit  writeSSS(FREE);

                return;
            }


            break;
        }

        }
    }
    else
    {
        Number_1.append(data);

        if(Number_1.count() > 3)
        {

            setNumber = false;

            if(Mod_Onix == CALL && Mod_SSS == FREE)
            {
                emit log(QString::number(index) +") OnixB принял Номер: <b>"+ QString::number(Number_1.value(0),16) +" " + QString::number(Number_1.value(1),16) +" " + QString::number(Number_1.value(2),16)+" " + QString::number(Number_1.value(3),16) +"</b>\n");

                timerCount = 0;

                Mod_Onix = IS ;
                Mod_SSS = IS;

                emit IS_Gui(true);

                emit writeSSS(IS); // РАБОТА

                return;

            }

            if(Mod_Onix == FREE && Mod_SSS == RECEIPT )
            {


                timerCount++;

                if(timerCount >5)
                {
                    timerCount = 0;

                    Mod_Onix = IS ;
                    Mod_SSS = IS;

                    emit IS_Gui(true);

                    emit writeSSS(IS); // РАБОТА
                }
                else
                {
               //     emit log(QString::number(index) +") OnixB принял Номер: <b>"+ QString::number(Number_1.value(0),16) +" " + QString::number(Number_1.value(1),16) +" " + QString::number(Number_1.value(2),16)+" " + QString::number(Number_1.value(3),16) +"</b>\n");

                    emit log(QString::number(index) +") OnixB принял Номер: <b>"+ nomer(Number_1) +"</b>\n");


                    emit log("Таймер : " + QString::number(timerCount) + " c");
                }

                return;
            }


         //   emit log(QString::number(index) +") OnixB принял Номер: <b>"+ QString::number(Number_1.value(0),16) +" " + QString::number(Number_1.value(1),16) +" " + QString::number(Number_1.value(2),16)+" " + QString::number(Number_1.value(3),16) +"</b>\n");

            emit log(QString::number(index) +") OnixB принял Номер: <b>"+ nomer(Number_1) +"</b>\n");




            qDebug () << "Принял Номер: ";
            qDebug () << Number_1;

            if(Mod_Onix == CALL2 && Mod_SSS == PPV)
            {

                emit log(QString::number(index) +") OnixB отправил команду <b>ППО</b>");

                Mod_Onix = CALL2Replay;

                emit writeSSS(CALL2Replay); // CALL2Replay

                return;
            }

            if(Mod_Onix == RADIO && Mod_SSS == CALL2)
            {
                for(int i=0; i < 4;i++)
                {
                    if(DefaultNumber_2.value(i) != Number_1.value(i))
                    {
                        flagNumber = true;
                    }
                }

                //ОТБОЙ
                if(flagNumber)
                {
                    emit log(QString::number(index) + ") OnixB отправил  команду <b>ОТБОЙ</b>.\n");

                    Mod_Onix = HANGUP;
                    Mod_SSS = HANGUP;
                    flagNumber = false;

                    emit writeSSS(HANGUP); // Отбой

                    return;
                }

         //       emit log(QString::number(index) +") OnixB отправил команду <b>ППВ</b> Номер: <b>"+ QString::number(Number_1.value(0),16) +" " + QString::number(Number_1.value(1),16) +" " + QString::number(Number_1.value(2),16)+" " + QString::number(Number_1.value(3),16)+"</b>\n");

                emit log(QString::number(index) +") OnixB отправил команду <b>ППВ</b> Номер: <b>"+ nomer(Number_1) +"</b>\n");

                Mod_Onix = PPV;

                emit writeSSS(PPV); // PPV

                emit writeSSS(Number_1.value(0));
                emit writeSSS(Number_1.value(1));
                emit writeSSS(Number_1.value(2));
                emit writeSSS(Number_1.value(3));

                return;

            }


            if(Mod_Onix == FREE && Mod_SSS == CALL)
            {

                for(int i=0; i < 4;i++)
                {
                    if(DefaultNumber_1.value(i) != Number_1.value(i))
                    {
                        flagNumber = true;
                    }
                }

                //ОТБОЙ
                if(flagNumber)
                {
                    emit log(QString::number(index) + ") OnixB отправил  команду <b>ОТБОЙ</b>.\n");

                    Mod_Onix = HANGUP;
                    Mod_SSS = HANGUP;
                    flagNumber = false;

                    emit writeSSS(HANGUP); // Отбой

                    return;
                }


                if(Mod_Onix_ms == BUSY && Mod_SSS == CALL)
                {
                    emit log(QString::number(index) + ") OnixB отправил команду <b>ЗАНЯТО</b>.\n");

                    Mod_Onix = BUSY;

                    emit Free_Gui(true);

                    emit writeSSS(BUSY); // Занято
                    return;
                }


              //  emit log(QString::number(index) + ") OnixB отправил команду <b>РАБОТА</b>.\n Номер: <b>" + QString::number(Number_1.value(0),16) +" " + QString::number(Number_1.value(1),16) +" " + QString::number(Number_1.value(2),16)+" " + QString::number(Number_1.value(3),16) +"</b>\n");

                emit log(QString::number(index) + ") OnixB отправил команду <b>РАБОТА</b>.\n Номер: <b>" + nomer(Number_1) +"</b>\n");





                Mod_Onix = WORK;

                emit writeSSS(WORK); // РАБОТА

                emit writeSSS(Number_1.value(0));
                emit writeSSS(Number_1.value(1));
                emit writeSSS(Number_1.value(2));
                emit writeSSS(Number_1.value(3));

                return;
            }

        }
    }
}

void OnixB::clearAll()
{
    Mod_Onix = FREE;
    Mod_SSS = FREE;

    Mod_Onix_ms = FREE;

    setNumber = false;
    timerCount = 0;
    flagNumber = false;
}

void OnixB::comandCall(QList<quint16> number_1,QList<quint16> number_2)
{
    Number_1.clear();
    Number_2.clear();

    Mod_Onix = CALL;

  //  emit log(QString::number(index) + ") OnixB отправил  команду <b>ВЫЗОВ</b> авбонента.\n Номер: <b>" + QString::number(number_1.value(0),16) +"\n" + QString::number(number_1.value(1),16) +"\n" + QString::number(number_1.value(2),16)+"\n" + QString::number(number_1.value(3),16) +"</b>\n");

     emit log(QString::number(index) + ") OnixB отправил  команду <b>ВЫЗОВ</b> авбонента.\n Номер: <b>" + nomer(number_1)+"</b>\n");

    qDebug () << index <<") OnixB отправил  команду Вызов авбонента.";


    emit writeSSS(CALL); // Вызов

    // Номер абонента
    for(int i=0; i < 4;i++)
    {
        Number_1.append(number_1[i]);
        Number_2.append(number_2[i]);
    }


    for(int i=0; i < Number_1.count()-1;i++)
    {
        if((Number_1.value(i) & 0xFF)  == (Number_1.value(i+1) & 0xFF))
        {
            switch (i) {
            case 0:
            {
                Number_1[i+1] = REPEAT_1;
                break;
            }
            case 1:
            {
                Number_1[i+1] = REPEAT_2;
                break;
            }
            case 2:
            {
                Number_1[i+1] = REPEAT_3;
                break;
            }
            }
        }
    }


    for(int i=0; i < Number_2.count()-1;i++)
    {
        if((Number_2.value(i) & 0xFF) == (Number_2.value(i+1) & 0xFF))
        {
            switch (i) {
            case 0:
            {
                Number_2[i+1] = REPEAT_1;
                break;
            }
            case 1:
            {
                Number_2[i+1] = REPEAT_2;
                break;
            }
            case 2:
            {
                Number_2[i+1] = REPEAT_3;
                break;
            }
            }
        }
    }

    emit writeSSS(Number_1.value(0));
    emit writeSSS(Number_1.value(1));
    emit writeSSS(Number_1.value(2));
    emit writeSSS(Number_1.value(3));

}

void OnixB::cansel()
{
    Number_1.clear();
    Number_2.clear();

    Mod_Onix = HANGUP;

    emit log(QString::number(index) + ") OnixB отправил  команду <b>ОТБОЙ</b>\n");

    qDebug () << index <<") OnixB отправил  команду ОТБОЙ авбонента.";


    emit writeSSS(HANGUP); // Вызов
}

void OnixB::setNumber_1(QList<quint16> number_1, QList<quint16> number_2)
{
    DefaultNumber_1.clear();
    DefaultNumber_2.clear();

    for(int i=0; i < number_1.count();i++)
    {
        DefaultNumber_1.append(number_1.value(i));
        DefaultNumber_2.append(number_2.value(i));
    }


    for(int i=0; i < DefaultNumber_1.count()-1;i++)
    {
        if((DefaultNumber_1.value(i) & 0xFF) == (DefaultNumber_1.value(i+1) & 0xFF))
        {
            switch (i) {
            case 0:
            {
                DefaultNumber_1[i+1] = REPEAT_1;
                break;
            }
            case 1:
            {
                DefaultNumber_1[i+1] = REPEAT_2;
                break;
            }
            case 2:
            {
                DefaultNumber_1[i+1] = REPEAT_3;
                break;
            }
            }
        }
    }


    for(int i=0; i < DefaultNumber_2.count()-1;i++)
    {
        if((DefaultNumber_2.value(i) & 0xFF) == (DefaultNumber_2.value(i+1) & 0xFF))
        {
            switch (i) {
            case 0:
            {
                DefaultNumber_2[i+1] = REPEAT_1;
                break;
            }
            case 1:
            {
                DefaultNumber_2[i+1] = REPEAT_2;
                break;
            }
            case 2:
            {
                DefaultNumber_2[i+1] = REPEAT_3;
                break;
            }
            }
        }
    }
}

void OnixB::setMod_Onix(quint16 data)
{
    Mod_Onix_ms = data;
}

QString OnixB::nomer(QList<quint16> number)
{
    QString list = "";

    for(int i=0; i < number.count(); i++)
    {
        switch (number.value(i) & 0xFF) {
        case 0x58: list += "0";break;
        case 0x8C: list += "1";break;
        case 0x4C: list += "2";break;
        case 0xC8: list += "3";break;
        case 0x2C: list += "4";break;
        case 0xA8: list += "5";break;
        case 0x68: list += "6";break;
        case 0x38: list += "7";break;
        case 0x1C: list += "8";break;
        case 0x98: list += "9";break;
        default: list +=  "ПОВТОР";break; //list[list.count()-1];break;

        }
    }




    return list;
}


// «РЕГИСТРАЦИЯ», «ПОДТВЕРЖДЕНИЕ РЕГИСТРАЦИИ», «ОТКАЗ ОТ РЕГИСТРАЦИИ», «ПОДТВЕРЖДЕНИЕ ОТКАЗА ОТ РЕГИСТРАЦИИ»
void OnixB::ISS3_Receipt_fn(quint8 CNS, quint8 numberSendMSB, quint8 numberSendLSB, quint8 RchVK, quint8 Priorytet)
{
    QByteArray mass;

    mass.append(static_cast<char>(CNS));
    mass.append(static_cast<char>(numberSendMSB));
    mass.append(static_cast<char>(numberSendLSB));
    mass.append(static_cast<char>(RchVK));
    mass.append(static_cast<char>(Priorytet));

    ISS3_Receipt *iss3 = (ISS3_Receipt*)mass.data();

       qDebug() << QString::number(iss3->comand, 16);
       qDebug() << QString::number(iss3->nbp, 16);
       qDebug() << QString::number(iss3->speed, 16);
       qDebug() << QString::number(iss3->numberSendMSB, 16) << QString::number(iss3->numberSendLSB, 16);
       qDebug() << QString::number(iss3->RChVK, 16);
       qDebug() << QString::number(iss3->Rezerv1, 16);
       qDebug() << QString::number(iss3->Priopy, 16);
       qDebug() << QString::number(iss3->Rezerv2, 16);


       for (int i=0; i < mass.count();i++) {

           emit writeSSS_ISS3(static_cast<quint8>(mass[i]));
       }



}

//«ВЫЗОВ», «ОТВЕТ РТР», «ОТВЕТ ЗС», «ОТБОЙ ЗС», «ОТБОЙ ПО ПРИОРИТЕТУ», «ЗАНЯТА ЗС»
void OnixB::ISS3_Call_fn(quint8 CNS, quint8 numberSendMSB, quint8 numberSendLSB, quint8 numberReceiverMSB, quint8 numberReceiverLSB)
{
    QByteArray mass;

    mass.append(static_cast<char>(CNS));
    mass.append(static_cast<char>(numberSendMSB));
    mass.append(static_cast<char>(numberSendLSB));
    mass.append(static_cast<char>(numberReceiverMSB));
    mass.append(static_cast<char>(numberReceiverLSB));

    ISS3_Call *iss3 = (ISS3_Call*)mass.data();

       qDebug() << QString::number(iss3->comand, 16);
       qDebug() << QString::number(iss3->nbp, 16);
       qDebug() << QString::number(iss3->speed, 16);
       qDebug() << QString::number(iss3->numberSendMSB, 16) << QString::number(iss3->numberSendLSB, 16);
       qDebug() << QString::number(iss3->numberReceiverMSB, 16) << QString::number(iss3->numberReceiverLSB, 16);


       for (int i=0; i < mass.count();i++) {

           emit writeSSS_ISS3(static_cast<quint8>(mass[i]));
       }

}

// «СВОБОДЕН»
void OnixB::ISS3_Free_fn(quint8 CNS, quint8 numberSendMSB, quint8 numberSendLSB, quint8 Rezerv1, quint8 Rezerv2)
{
    QByteArray mass;

    mass.append(static_cast<char>(CNS));
    mass.append(static_cast<char>(numberSendMSB));
    mass.append(static_cast<char>(numberSendLSB));
    mass.append(static_cast<char>(Rezerv1));
    mass.append(static_cast<char>(Rezerv2));

    ISS3_Free *iss3 = (ISS3_Free*)mass.data();

       qDebug() << QString::number(iss3->comand, 16);
       qDebug() << QString::number(iss3->nbp, 16);
       qDebug() << QString::number(iss3->speed, 16);
       qDebug() << QString::number(iss3->numberSendMSB, 16) << QString::number(iss3->numberSendLSB, 16);
       qDebug() << QString::number(iss3->Rezerv1, 16) << QString::number(iss3->Rezerv2, 16);


       for (int i=0; i < mass.count();i++) {

           emit writeSSS_ISS3(static_cast<quint8>(mass[i]));
       }
}

void OnixB::ISS3_ISS3_Refusal_fn(quint8 CNS, quint8 numberSendMSB, quint8 numberSendLSB, quint8 numberReceiverMSB, quint8 numberReceiverLSB)
{
    QByteArray mass;

    mass.append(static_cast<char>(CNS));
    mass.append(static_cast<char>(numberSendMSB));
    mass.append(static_cast<char>(numberSendLSB));
    mass.append(static_cast<char>(numberReceiverMSB));
    mass.append(static_cast<char>(numberReceiverLSB));

    ISS3_Refusal *iss3 = (ISS3_Refusal*)mass.data();

       qDebug() << QString::number(iss3->comand, 16);
       qDebug() << QString::number(iss3->why, 16);
       qDebug() << QString::number(iss3->numberSendMSB, 16) << QString::number(iss3->numberSendLSB, 16);
       qDebug() << QString::number(iss3->numberReceiverMSB, 16) << QString::number(iss3->numberReceiverLSB, 16);


       for (int i=0; i < mass.count();i++) {

           emit writeSSS_ISS3(static_cast<quint8>(mass[i]));
       }
}

