#include "sss.h"

Sss::Sss(QObject *parent) : QObject(parent),
    Mod_Onix_1(quint16(FREE)),
    Mod_SSS_1(quint16(FREE)),
    Mod_Onix_2(quint16(FREE)),
    Mod_SSS_2(quint16(FREE)),
    kol(0),
    kol2(0)
{

    this->moveToThread(new QThread());

    connect(this->thread(),&QThread::started,this,&Sss::process_start);

    this->thread()->start();

}

void Sss::process_start()
{

    //Инициализация таймеров
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    timer = new QTimer();
    connect(timer,&QTimer::timeout,this,&Sss::free);

    Number_1.clear();
    Number_2.clear();


    setNumber = false;
    setNumber_1= false;
    count = 0;

}

void Sss::readSSS(quint16 data)
{

    if(setNumber_1 == false)
    {

       /* kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");
*/

        switch (data) {

        case RADIO:
        {
            if(Mod_SSS_1 == FREE && Mod_Onix_1 == WORK)
            {
                emit log1("1) SSS принял Команду <b>РАДИО<b>.\n");

                qDebug () << "1) SSS принял Команду РАДИО";

                Mod_Onix_1 = RADIO;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = RADIO;
                Mod_Onix_2 = FREE;

            }


            break;
        }


        case CALL2Replay:
        {
            if(Mod_Onix_1 == NULL && Mod_SSS_1 == PPV)
            {
                emit log1("1) SSS принял Команду <b>ППО</b>\n");

                qDebug () << "1) SSS принял Команду ППО";

                Mod_Onix_1 = CALL2Replay;
                Mod_SSS_1 = PPV;
                Mod_SSS_2 = CALL2Replay;
                Mod_Onix_2 = PPV;

                return;
            }
            break;
        }

        case CALL2:
        {
            if(Mod_Onix_1 == FREE && Mod_SSS_1 == RADIO && Mod_SSS_2 == FREE && Mod_Onix_2 ==RADIO)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ2</b>\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ 2";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }

            if(Mod_Onix_1 == RADIO && Mod_SSS_1 == CALL2 && Mod_SSS_2 == RADIO && Mod_Onix_2 ==CALL2)
            {
                emit log1("2) SSS принял Команду <b>ППВ</b>\n");

                qDebug () << "2) SSS принял Команду ППВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }

            break;
        }


        case BUSY:
        {
            if(Mod_Onix_1 == FREE && Mod_SSS_1 == CALL && Mod_SSS_2 == RECEIPT && Mod_Onix_2 == CALL)
            {
                emit log1("1) SSS принял Команду <b>ЗАНЯТО<b>.\n");

                qDebug () << "1) SSS принял Команду ЗАНЯТО";


                Mod_Onix_1 = BUSY;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = BUSY;
                Mod_Onix_2 = FREE;

                return;
            }

            if(Mod_Onix_1 == FREE && Mod_SSS_1 == CALL && Mod_SSS_2 == RECEIPT && Mod_Onix_2 == FREE)
            {
                emit log1("1) SSS принял Команду <b>ЗАНЯТО<b>.\n");

                qDebug () << "1) SSS принял Команду ЗАНЯТО";


                Mod_Onix_1 = BUSY;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = BUSY;
                Mod_Onix_2 = FREE;

                return;
            }


            break;
        }

        case CALL: // WORK
        {

            //Режим фазирования  РАБОТА (принятие)
            if(Mod_Onix_1 == FREE &&  Mod_SSS_1 == CALL && Mod_SSS_mod == PHASING &&  Mod_SSS_2 == RECEIPT && Mod_Onix_2 == FREE)
            {
                emit log1("1) SSS принял Команду <b>РАБОТА</b>.\n");

                qDebug () << "1) SSS принял Команду РАБОТА";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }



            //Режим фазирования СТАТУС ССС «ОТБОЙ»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == HANGUP && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }


            //Режим фазирования СТАТУС ССС «ЗАНЯТО»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == NULL &&  Mod_SSS == BUSY && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }

            //Режим фазирования СТАТУС ССС «ЗАНЯТО (ФЗ)»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == NULL && Mod_SSS == BUSY_PHASING && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }




            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ (ФЗ)»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == BUSY_PHASING && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }

            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ» СВОБОДЕН
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING  && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE && Mod_SSS_fz == NULL && Mod_SSS == BUSY_PHASING)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }


            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ» ИС
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING  && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }

            //Режим фазирования РАБОТА
            if(Mod_Onix_1 == FREE && Mod_SSS == WORK && Mod_SSS_2 == PHASING && Mod_Onix_2 ==FREE)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }


            //Режим фазирования ВЫЗОВ
            if(Mod_Onix_1 == FREE &&  Mod_SSS_1 == FREE && Mod_SSS_mod == PHASING &&  Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }


            //Квитанция

            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ» ИС (Квитанция)
            if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_mod == RECEIPT  && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }



            if(Mod_Onix_1 == FREE && Mod_SSS == WORK && Mod_SSS_2 == RECEIPT && Mod_Onix_2 ==FREE)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }

            if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == FREE && Mod_Onix_2 ==FREE)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }

            if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
            {
                emit log1("1) SSS принял Команду <b>ВЫЗОВ</b> .\n");

                qDebug () << "1) SSS принял Команду ВЫЗОВ.";

                Number_1.clear();
                setNumber_1 = true;
                return;
            }


            if(Mod_Onix_1 == FREE && Mod_SSS_1 == CALL && Mod_SSS_2 == RECEIPT && Mod_Onix_2 == CALL)
            {
                emit log1("1) SSS принял Команду <b>РАБОТА</b>.\n");

                qDebug () << "1) SSS принял Команду РАБОТА.";

                Mod_Onix_1 = WORK;

                Number_1.clear();

                setNumber_1 = true;
                return;
            }

            if(Mod_Onix_1 == CALL && Mod_SSS_1 == FREE && Mod_SSS_2 == CALL && Mod_Onix_2 == CALL)
            {
                emit log1("1) SSS принял Команду <b>РАБОТА</b>.\n");

                qDebug () << "1) SSS принял Команду РАБОТА.";

                Mod_Onix_1 = WORK;

                Number_1.clear();

                setNumber_1 = true;
                return;
            }


            break;
        }

        case HANGUP:
        {

            if(Mod_Onix_1 == FREE &&  Mod_SSS_1 == RECEIPT && Mod_SSS_mod == PHASING &&  Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log1("1) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = HANGUP;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = FREE;
                return;
            }


            if(Mod_Onix_1 == CALL && Mod_SSS_1 ==  CALL && Mod_SSS_2 == FREE && Mod_Onix_2 ==  CALL)
            {
                emit log1("1) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = HANGUP;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = WORK;
                return;
            }

            if(Mod_Onix_1 == FREE && Mod_SSS_1 == WORK && Mod_SSS_2 == RECEIPT && Mod_Onix_2 == WORK)
            {
                emit log1("1) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = HANGUP;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = FREE;
                return;
            }

            if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
            {
                emit log1("1) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = HANGUP;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = IS;
                return;
            }

            if(Mod_Onix_1 == FREE && Mod_SSS_1 == RECEIPT && Mod_SSS_2 == CALL && Mod_Onix_2 == FREE && Mod_SSS_mod == PHASING)
            {
                emit log1("1) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = HANGUP;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = FREE;
                return;
            }

            if(Mod_Onix_1 == FREE && Mod_SSS_1 == RADIO && Mod_SSS_2 == FREE && Mod_Onix_2 == RADIO)
            {
                emit log1("1) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = HANGUP;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = FREE;
                return;
            }
            // Отбой на ППВ
            if(Mod_Onix_1 == CALL2Replay && Mod_SSS_1 == CALL2 && Mod_SSS_2 == CALL2Replay && Mod_Onix_2 == CALL2)
            {
                emit log1("1) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = HANGUP;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = FREE;
                return;
            }

            // Отбой на РАДИО
            if(Mod_Onix_1 == PPV && Mod_SSS_1 == RADIO && Mod_SSS_2 == CALL2 && Mod_Onix_2 == RADIO)
            {
                emit log1("1) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = HANGUP;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = FREE;
                return;
            }






            break;
        }

        case FREE:
        {
            if(Mod_Onix_1 == HANGUP && Mod_SSS_1 == HANGUP)
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }


            //Квитанция
             if(Mod_Onix_1 == CALL  && Mod_SSS_1 == RECEIPT && Mod_SSS_mod == RECEIPT && Mod_SSS == BUSY_PHASING)
             {
                 emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                 Mod_Onix_1 = FREE;
                 Mod_SSS_1 = BUSY_PHASING;

                 return;
             }

             if(Mod_Onix_1 == CALL && Mod_SSS_1 == PHASING && Mod_SSS_mod == PHASING && Mod_SSS == BUSY_PHASING && Mod_SSS_fz == NULL)
             {
                 emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                 Mod_Onix_1 = FREE;
                 Mod_SSS_1 = RECEIPT;

                 return;
             }


            if(Mod_Onix_1 == FREE && Mod_SSS_1 == HANGUP)
            {

                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }

            if(Mod_Onix_1 == HANGUP && Mod_SSS_1 == IS)
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }

            if(Mod_Onix_1 == IS && Mod_SSS_1 == HANGUP )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }

            if(Mod_SSS_1 == FREE && Mod_Onix_1 == BUSY)
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }

            if(Mod_SSS_1 == BUSY && Mod_Onix_1 == FREE)
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }

            if(Mod_Onix_1 == IS && Mod_SSS_1 == BUSY )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }

            if(Mod_Onix_1 == IS && Mod_SSS_1 == PRIORITY_CALL_OFF )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }

            if(Mod_Onix_1 == CALL && Mod_SSS_1 == BUSY )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }

            if(Mod_Onix_1 == CALL && Mod_SSS_1 == HANGUP )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }

            if(Mod_Onix_1 == CALL && Mod_SSS_1 == BUSY_PHASING )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }


             //Фазирование СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ»
            if(Mod_Onix_1 == CALL && Mod_SSS == BUSY_PHASING && Mod_SSS_mod == PHASING  && Mod_SSS_fz == NULL && Mod_SSS_2 == FREE  && Mod_Onix_2 == FREE )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = FREE;

                return;
            }



            //Фазирование отбой ВЫЗОВА
            if(Mod_Onix_1 == HANGUP && Mod_SSS_1 == HANGUP && Mod_SSS == FREE  && Mod_SSS_fz == FREE )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }

            //Фазирование СТАТУС ССС «ОТБОЙ»
            if(Mod_Onix_1 == CALL && Mod_SSS_1 == PHASING && Mod_SSS == HANGUP  && Mod_SSS_fz == HANGUP )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = RECEIPT;

                return;
            }

            //Фазирование СТАТУС ССС «ОТБОЙ»
            if(Mod_Onix_1 == FREE && Mod_SSS_1 == RECEIPT && Mod_SSS == HANGUP  && Mod_SSS_fz == HANGUP )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = HANGUP;

                return;
            }



            //Фазирование СТАТУС ССС «ЗАНЯТО (ФЗ)»
            if(Mod_Onix_1 == CALL && Mod_SSS_1 == PHASING && Mod_SSS == BUSY  && Mod_SSS_fz == BUSY )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = BUSY;

                return;
            }

            //Фазирование СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ (ФЗ)»
            if(Mod_Onix_1 == CALL && Mod_SSS_1 == PHASING && Mod_SSS == BUSY_PHASING  && Mod_SSS_fz == BUSY_PHASING )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = BUSY_PHASING;

                return;
            }


          /*  //Фазирование СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ»
            if(Mod_Onix_1 == CALL && Mod_SSS_1 == PHASING )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = RECEIPT;

                return;
            }*/

            //Фазирование занят физ вых.
            if(Mod_Onix_1 == FREE && Mod_SSS == BUSY_PHASING  && Mod_SSS_1 == RECEIPT)
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = BUSY_PHASING;

                return;
            }

            //Фазирование занят физ вых.
            if(Mod_Onix_1 == FREE && Mod_SSS_1 == BUSY_PHASING)
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;

                return;
            }


            //Фазирование ВЫЗОВ до конца
            if(Mod_Onix_1 == CALL && Mod_SSS_1 == PHASING && Mod_SSS_mod == PHASING  && Mod_SSS_2 == FREE  && Mod_Onix_2 == FREE )
            {
                emit log1("1) SSS принял Команду <b>СВОБОДЕН</b>\n");

                switch (Mod_SSS) {

                    case WORK:
                    {
                    Mod_Onix_1 = FREE;
                    Mod_SSS_1  =  RECEIPT;
                    Mod_SSS_2  =  FREE;
                        return;
                    }

                }


                Mod_Onix_1 = FREE;
                Mod_SSS_1 = RECEIPT;
                Mod_SSS_2 = CALL;

                return;
            }





            break;
        }

        case IS:
        {
            if(Mod_Onix_1 == WORK && Mod_SSS_1 == WORK && Mod_SSS_2 == FREE && Mod_Onix_2 == WORK)
            {
                emit log1("1) SSS принял Команду <b>ИС</b>\n");


                Mod_Onix_1 = IS;
                Mod_SSS_1 = IS;
                Mod_SSS_2 = IS;
                Mod_Onix_2 = IS;
                return;
            }

            if(Mod_Onix_1 == PPV && Mod_SSS_1 == CALL2Replay && Mod_SSS_2 == PPV && Mod_Onix_2 == CALL2Replay)
            {
                 emit log1("1) SSS принял Команду <b>ИС</b>\n");

                Mod_Onix_1 = IS;
                Mod_SSS_1 = IS;
                Mod_SSS_2 = IS;
                Mod_Onix_2 = IS;
                return;
            }

            if(Mod_Onix_1 == FREE && Mod_SSS_1 == WORK && Mod_SSS_2 == WORK && Mod_Onix_2 == FREE)
            {
                 emit log1("1) SSS принял Команду <b>ИС</b>\n");

                Mod_Onix_1 = IS;
                Mod_SSS_1 = IS;
                Mod_SSS_2 = IS;
                Mod_Onix_2 = IS;
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

            setNumber_1 = false;

     //       emit log1(QString::number(1) + " ) SSS принял Номер: <b>"+ QString::number(Number_1.value(0),16) +"\n" + QString::number(Number_1.value(1),16) +"\n" + QString::number(Number_1.value(2),16)+"\n" + QString::number(Number_1.value(3),16) +"</b>\n");

             emit log1(QString::number(1) + " ) SSS принял Номер: <b>"+ nomer(Number_1)+"</b>\n");





            qDebug () << "1) SSS принял Номер: ";
            qDebug () << Number_1;

            //Режим фазирования  РАБОТА (принятие)
            if(Mod_Onix_1 == FREE &&  Mod_SSS_1 == CALL && Mod_SSS_mod == PHASING &&  Mod_SSS_2 == RECEIPT && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = CALL;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = WORK;
                Mod_Onix_2 = WORK;

                return;
            }




            //Режим фазирования СТАТУС ССС «ОТБОЙ»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == HANGUP && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = CALL;
                Mod_SSS_1 = PHASING;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = FREE;

                return;
            }

            //Режим фазирования СТАТУС ССС «ЗАНЯТО»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == NULL &&  Mod_SSS == BUSY && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = CALL;
                Mod_SSS_1 = BUSY;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = FREE;

                return;
            }


            //Режим фазирования СТАТУС ССС «ЗАНЯТО (ФЗ)»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == BUSY && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = CALL;
                Mod_SSS_1 = PHASING;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = FREE;

                return;
            }

            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ» СВОБОДЕН
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS == BUSY_PHASING && Mod_SSS_fz == NULL  && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = CALL;
                Mod_SSS_1 = BUSY_PHASING;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = FREE;

                return;
            }

            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ (ФЗ)»
            if(Mod_Onix_1 == FREE && Mod_SSS == BUSY_PHASING  && Mod_SSS_mod == PHASING && Mod_SSS_fz == BUSY_PHASING && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = CALL;
                Mod_SSS_1 = PHASING;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = FREE;

                return;
            }

            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
            {
                Mod_Onix_1 = CALL;
                Mod_SSS_1 = PHASING;
                Mod_SSS_2 = IS;
                Mod_Onix_2 = IS;

                return;
            }

            //Режим фазирования РАБОТА
            if(Mod_Onix_1 == FREE && Mod_SSS == WORK && Mod_SSS_mod  == PHASING  && Mod_SSS_2 == RECEIPT && Mod_Onix_2 ==FREE)
            {
                Mod_Onix_1 = FREE;
                Mod_SSS_1 = WORK;
                Mod_SSS_2 = WORK;
                Mod_Onix_2 = FREE;

                return;
            }



            //Режим фазирования Вызов
             if(Mod_Onix_1 == FREE &&  Mod_SSS_2 == FREE && Mod_SSS_mod == PHASING &&  Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
             {


                 switch (Mod_SSS) {

                     case WORK:
                     {
                         Mod_Onix_1 = CALL;
                         Mod_SSS_1  =  PHASING;
                         Mod_SSS_2 = FREE;
                         Mod_Onix_2 = FREE;
                         return;
                     }

                 }


                 Mod_Onix_1 = CALL;
                 Mod_SSS_1 = PHASING;
                 Mod_SSS_2 = FREE;
                 Mod_Onix_2 = FREE;

                 return;
             }


            //Квитанция


            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ» ИС (Квитанция)
            if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_mod == RECEIPT  && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
            {
                Mod_Onix_1 = CALL;
                Mod_SSS_1 = RECEIPT;
                Mod_SSS_2 = IS;
                Mod_Onix_2 = IS;

                 return;
            }


            if(Mod_Onix_1 == FREE && Mod_SSS == WORK && Mod_SSS_2 == RECEIPT && Mod_Onix_2 ==FREE)
            {
                Mod_Onix_1 = FREE;

                Mod_SSS_1 = WORK;

                Mod_SSS_2 = WORK;

                Mod_Onix_2 = FREE;

                return;
            }

            if(Mod_Onix_1 == FREE && Mod_SSS_1 == RADIO && Mod_SSS_2 == FREE && Mod_Onix_2 ==RADIO)
            {
                Mod_Onix_1 = CALL2;

                Mod_SSS_1 = RADIO;

                Mod_SSS_2 = CALL2;

                Mod_Onix_2 = RADIO;
            }

            if(Mod_Onix_1 == RADIO && Mod_SSS_1 == CALL2 && Mod_SSS_2 == RADIO && Mod_Onix_2 ==CALL2)
            {
                Mod_Onix_1 = PPV;
                Mod_SSS_1 = NULL;
                Mod_SSS_2 = PPV;
                Mod_Onix_2 = NULL;
            }


            if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == IS && Mod_Onix_2 ==IS)
            {
                switch (Mod_SSS) {

                    case BUSY_PHASING:
                    {
                        Mod_SSS_1 = BUSY_PHASING;
                        Mod_Onix_1 = CALL;
                        return;
                    }

                }
            }


            //Вызов со стороны OnixB 1
            if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                switch (Mod_SSS) {

                    case WORK:
                    {
                        Mod_SSS_1  =  RECEIPT;
                        Mod_SSS_2  =  FREE;

                        return;
                    }

                    case BUSY:
                    {
                        Mod_SSS_1 = BUSY;
                        Mod_Onix_1 = CALL;
                        return;
                    }

                    case HANGUP:
                    {
                        Mod_SSS_1 = HANGUP;
                        Mod_Onix_1 = CALL;
                        return;
                    }

                    case BUSY_PHASING:
                    {
                        Mod_SSS_1 = BUSY_PHASING;
                        Mod_Onix_1 = CALL;
                        return;
                    }

                }

                Mod_Onix_1 = CALL;

                Mod_SSS_1 = RECEIPT;

                Mod_SSS_2 = CALL;

            }

            //Работа к OnixB 2
            if(Mod_Onix_1 == WORK && Mod_SSS_1 == CALL && Mod_SSS_2 == RECEIPT && Mod_Onix_2 ==CALL)
            {
                Mod_Onix_1 = CALL;

                Mod_SSS_1 = FREE;

                Mod_SSS_2 = WORK;

            }


        }
    }
}

void Sss::readSSS2(quint16 data)
{
    if(setNumber == false)
    {
       /* kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");
        */

        switch (data) {

        case CALL2Replay:
        {
            if(Mod_Onix_2 == NULL && Mod_SSS_2 == PPV)
            {
                emit log2("2) SSS принял Команду <b>ППО</b>\n");

                qDebug () << "2) SSS принял Команду ППО";

                Mod_Onix_1 = PPV;
                Mod_SSS_1 = CALL2Replay;
                Mod_SSS_2 = PPV;
                Mod_Onix_2 = CALL2Replay;

                return;
            }
            break;
        }


        case PPV: //(CALL2)
        {

            if(Mod_Onix_1 == CALL2 && Mod_SSS_1 == RADIO && Mod_SSS_2 == CALL2 && Mod_Onix_2 ==RADIO)
            {
                emit log2("2) SSS принял Команду <b>ППВ</b>\n");

                qDebug () << "2) SSS принял Команду ППВ";

                Number_2.clear();
                setNumber = true;
                return;
            }

            if(Mod_Onix_1 == RADIO && Mod_SSS_1 == FREE && Mod_SSS_2 == RADIO && Mod_Onix_2 ==FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ2</b>\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ 2";

                Number_2.clear();
                setNumber = true;
                return;
            }

            break;
        }

        case RADIO:
        {
            if(Mod_SSS_2 == FREE && Mod_Onix_2 == WORK)
            {
                emit log2("2) SSS принял Команду <b>РАДИО</b>.\n");

                qDebug () << "2) SSS принял Команду РАДИО";

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = RADIO;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = RADIO;

            }


            break;
        }


        case BUSY:
        {
            if(Mod_Onix_1 == CALL && Mod_SSS_1 == RECEIPT && Mod_SSS_2 == CALL && Mod_Onix_2 ==FREE )
            {
                emit log2("2) SSS принял Команду <b>ЗАНЯТО<b>.\n");

                qDebug () << "2) SSS принял Команду ЗАНЯТО";


                Mod_Onix_1 = FREE;
                Mod_SSS_1 = BUSY;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = BUSY;

                return;
            }

            if(Mod_Onix_1 == FREE && Mod_SSS_1 == RECEIPT && Mod_SSS_2 ==CALL  && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ЗАНЯТО<b>.\n");

                qDebug () << "2) SSS принял Команду ЗАНЯТО";


                Mod_Onix_1 = FREE;
                Mod_SSS_1 = BUSY;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = BUSY;

                return;
            }

            break;
        }

        case CALL: //WORK
        {

            //Режим фазирования  РАБОТА (принятие)
            if(Mod_Onix_1 == FREE &&  Mod_SSS_1 == RECEIPT  && Mod_SSS_mod == PHASING &&  Mod_SSS_2 == CALL && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>РАБОТА</b>.\n");

                qDebug () << "2) SSS принял Команду РАБОТА";

                Number_2.clear();
                setNumber = true;
                return;
            }



            //Режим фазирования СТАТУС ССС «ОТБОЙ»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == HANGUP && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ";

                Number_2.clear();
                setNumber = true;
                return;
            }

            //Режим фазирования СТАТУС ССС «ЗАНЯТО»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == NULL && Mod_SSS == BUSY && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ";

                Number_2.clear();
                setNumber = true;
                return;
            }


            //Режим фазирования СТАТУС ССС «ЗАНЯТО (ФЗ)»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == BUSY && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ";

                Number_2.clear();
                setNumber = true;
                return;
            }

            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ (ФЗ)»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == BUSY_PHASING && Mod_SSS_1 == FREE && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ";

                Number_2.clear();
                setNumber = true;
                return;
            }

            //Режим фазирования
            if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_mod == PHASING && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ";

                Number_2.clear();
                setNumber = true;
                return;
            }

            //Режим фазирования РАБОТА
            if(Mod_Onix_1 == FREE && Mod_SSS == WORK && Mod_SSS_2 == WORK  && Mod_Onix_2 ==FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ";

                Number_2.clear();
                setNumber = true;
                return;
            }

            //Режим фазирования ВЫЗОВ
            if(Mod_Onix_1 == FREE &&  Mod_SSS_1 == RECEIPT &&  Mod_SSS== WORK && Mod_SSS_mod == PHASING  && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ";

                Number_2.clear();
                setNumber = true;
                return;
            }


            //Квитанция

            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ» ИС (Квитанция)
            if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_mod == RECEIPT  && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ";

                Number_2.clear();
                setNumber = true;
                return;
            }



            if(Mod_Onix_1 == FREE && Mod_SSS_1 == RECEIPT && Mod_SSS == WORK && Mod_Onix_2 ==FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ .";

                Number_2.clear();
                setNumber = true;
                return;
            }


            if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == FREE && Mod_Onix_2 ==FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ .";


                Number_2.clear();
                setNumber = true;
                return;
            }

            if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_2 == FREE && Mod_Onix_2 ==FREE)
            {
                emit log2("2) SSS принял Команду <b>ВЫЗОВ</b>.\n");

                qDebug () << "2) SSS принял Команду ВЫЗОВ .";

                Number_2.clear();
                setNumber = true;
                return;
            }

            //Принял команду работа
            if(Mod_Onix_1 == CALL && Mod_SSS_1 == RECEIPT && Mod_SSS_2 == CALL && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>РАБОТА</b>.\n");

                qDebug () << "2) SSS принял Команду РАБОТА.";

                Mod_Onix_2 = WORK;

                Number_2.clear();

                setNumber = true;
                return;
            }





            break;
        }

        case HANGUP:
        {

            if(Mod_Onix_1 == FREE &&  Mod_SSS_1 == FREE && Mod_SSS_mod == PHASING &&  Mod_SSS_2 == RECEIPT && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = HANGUP;
                return;
            }

            if(Mod_Onix_1 == CALL && Mod_SSS_1 ==  FREE && Mod_SSS_2 == CALL && Mod_Onix_2 ==  CALL)
            {
                emit log2("2) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = WORK;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = HANGUP;
                return;
            }

            if(Mod_Onix_1 == WORK && Mod_SSS_1 == RECEIPT && Mod_SSS_2 == WORK && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = HANGUP;
                return;
            }

            if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
            {
                emit log2("2) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = IS;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = HANGUP;
                return;
            }

            if(Mod_Onix_1 == CALL2 && Mod_SSS_1 == RADIO && Mod_SSS_2 == CALL2 && Mod_Onix_2 == RADIO)
            {
                emit log2("2) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = IS;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = HANGUP;
                return;
            }


            if(Mod_Onix_1 == FREE && Mod_SSS_1 == CALL  && Mod_SSS_2 == RECEIPT && Mod_Onix_2 == FREE && Mod_SSS_mod == PHASING)
            {
                emit log2("2) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = HANGUP;
                return;
            }

            if(Mod_Onix_1 == RADIO && Mod_SSS_1 == FREE && Mod_SSS_2 == RADIO && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = HANGUP;
                return;
            }

            // Отбой на ППВ
            if(Mod_Onix_1 == CALL2 && Mod_SSS_1 == CALL2Replay && Mod_SSS_2 == CALL2  && Mod_Onix_2 == CALL2Replay )
            {
                emit log2("2) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 = HANGUP;
                return;
            }

            // Отбой на РАДИО
            if(Mod_Onix_1 == RADIO && Mod_SSS_1 == CALL2 && Mod_SSS_2 == RADIO  && Mod_Onix_2 == PPV )
            {
                emit log2("2) SSS принял Команду <b>ОТБОЙ</b>.\n");

                Mod_Onix_1 = FREE ;
                Mod_SSS_1 = HANGUP;
                Mod_SSS_2 = HANGUP;
                Mod_Onix_2 =HANGUP;
                return;
            }


            break;
        }


        case IS:
        {
            if(Mod_Onix_1 == WORK && Mod_SSS_1 == FREE && Mod_SSS_2 == WORK && Mod_Onix_2 == WORK)
            {
                emit log2("2) SSS принял Команду <b>ИС</b>.\n");

                Mod_Onix_1 = IS;
                Mod_SSS_1 = IS;
                Mod_SSS_2 = IS;
                Mod_Onix_2 = IS;
                return;
            }

            if(Mod_Onix_1 == CALL2Replay && Mod_SSS_1 == PPV && Mod_SSS_2 == CALL2Replay && Mod_Onix_2 == PPV)
            {
                emit log2("2) SSS принял Команду <b>ИС</b>.\n");

                Mod_Onix_1 = IS;
                Mod_SSS_1 = IS;
                Mod_SSS_2 = IS;
                Mod_Onix_2 = IS;
                return;
            }

            if(Mod_Onix_1 == FREE && Mod_SSS_1 == WORK && Mod_SSS_2 == WORK && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>ИС</b>.\n");

                Mod_Onix_1 = IS;
                Mod_SSS_1 = IS;
                Mod_SSS_2 = IS;
                Mod_Onix_2 = IS;
                return;
            }


            break;
        }

        case FREE:
        {

            //Квитанция
             if(Mod_Onix_2 == CALL  && Mod_SSS_2 == RECEIPT && Mod_SSS_mod == RECEIPT && Mod_SSS == BUSY_PHASING)
             {
                 emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                 Mod_Onix_2 = FREE;
                 Mod_SSS_2 = BUSY_PHASING;

                 return;
             }


            if(Mod_SSS_2 == HANGUP && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>.\n");

                qDebug () << "2) SSS принял Команду СВОБОДЕН.";

                Mod_SSS_2 = FREE;
                Mod_Onix_2 = FREE;

                return;
            }

            if(Mod_SSS_2 == HANGUP && Mod_Onix_2 == HANGUP)
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = FREE;

                return;
            }

            if(Mod_SSS_2 == HANGUP && Mod_Onix_2 == IS)
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = FREE;

                return;
            }

            if(Mod_SSS_2 == FREE && Mod_Onix_2 == BUSY)
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = FREE;

                return;
            }

            if(Mod_SSS_2 == BUSY && Mod_Onix_2 == FREE)
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = FREE;

                return;
            }

            if(Mod_Onix_2 == IS && Mod_SSS_2 == BUSY )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = FREE;

                return;
            }

            if(Mod_Onix_2 == IS && Mod_SSS_2 == PRIORITY_CALL_OFF )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = FREE;

                return;
            }

            if(Mod_Onix_2 == CALL && Mod_SSS_2 == BUSY )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = FREE;

                return;
            }

            if(Mod_Onix_2 == CALL && Mod_SSS_2 == HANGUP )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = FREE;

                return;
            }

            if(Mod_Onix_2 == CALL && Mod_SSS_2 == BUSY_PHASING )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = FREE;

                return;
            }

            //Фазирование СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ»
           if(Mod_Onix_1 == FREE && Mod_SSS == BUSY_PHASING && Mod_SSS_mod == PHASING  && Mod_SSS_fz == NULL && Mod_SSS_2 == PHASING  && Mod_Onix_2 == CALL )
           {
               emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

               Mod_SSS_2 = FREE;
               Mod_Onix_2 = FREE;

               return;
           }



            //Фазирование СТАТУС ССС «ОТБОЙ»
            if(Mod_Onix_2 == CALL && Mod_SSS_2 == PHASING && Mod_SSS == HANGUP  && Mod_SSS_fz == HANGUP )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = RECEIPT;

                return;
            }

            //Фазирование отбой ВЫЗОВА
            if(Mod_Onix_2 == FREE && Mod_SSS_2 == RECEIPT && Mod_SSS == HANGUP  && Mod_SSS_fz == HANGUP )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = HANGUP;

                return;
            }


            //Фазирование СТАТУС ССС «ОТБОЙ»
            if(Mod_Onix_2 == FREE && Mod_SSS_2 == RECEIPT && Mod_SSS == HANGUP  && Mod_SSS_fz == HANGUP )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = HANGUP;

                return;
            }

            //Фазирование СТАТУС ССС «ЗАНЯТО (ФЗ)»
            if(Mod_Onix_2 == CALL && Mod_SSS_2 == PHASING && Mod_SSS == BUSY  && Mod_SSS_fz == BUSY )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = BUSY;

                return;
            }


            //Фазирование СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ (ФЗ)»
            if(Mod_Onix_2 == CALL && Mod_SSS_2 == PHASING && Mod_SSS == BUSY_PHASING  && Mod_SSS_fz == BUSY_PHASING )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = BUSY_PHASING;

                return;
            }


            if(Mod_Onix_2 == CALL && Mod_SSS_2 == PHASING && Mod_SSS_mod == PHASING && Mod_SSS == BUSY_PHASING && Mod_SSS_fz == NULL)
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = RECEIPT;

                return;
            }


            //Фазирование занят физ вых.
            if(Mod_Onix_2 == FREE && Mod_SSS == BUSY_PHASING  && Mod_SSS_2 == RECEIPT)
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = BUSY_PHASING;

                return;
            }

            //Фазирование занят физ вых.
            if(Mod_Onix_2 == FREE && Mod_SSS_2 == BUSY_PHASING)
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = FREE;

                return;
            }

            //Фазирование ВЫЗОВ до конца
            if(Mod_Onix_2 == CALL && Mod_SSS_2 == PHASING && Mod_SSS_mod == PHASING  && Mod_SSS_1 == FREE  && Mod_Onix_1 == FREE )
            {
                emit log2("2) SSS принял Команду <b>СВОБОДЕН</b>\n");

                switch (Mod_SSS) {

                    case WORK:
                    {
                    Mod_Onix_2 = FREE;
                    Mod_SSS_2  =  RECEIPT;
                    Mod_SSS_1  =  FREE;
                        return;
                    }

                }

                Mod_Onix_2 = FREE;
                Mod_SSS_2 = RECEIPT;
                Mod_SSS_1 = CALL;

                return;
            }





            break;
        }

        }
    }
    else
    {
        Number_2.append(data);

        if(Number_2.count() > 3)
        {

            setNumber = false;

        //    emit log2(QString::number(2) + " ) SSS принял Номер: <b>"+ QString::number(Number_2.value(0),16) +"\n" + QString::number(Number_2.value(1),16) +"\n" + QString::number(Number_2.value(2),16)+"\n" + QString::number(Number_2.value(3),16) +"</b>\n");

              emit log2(QString::number(2) + " ) SSS принял Номер: <b>"+ nomer(Number_2) +"</b>\n");



            qDebug () << "2) SSS принял Номер: ";
            qDebug () << Number_2;

            //Режим фазирования  РАБОТА (принятие)
            if(Mod_Onix_1 == FREE &&  Mod_SSS_1 == RECEIPT  && Mod_SSS_mod == PHASING &&  Mod_SSS_2 == CALL && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = WORK;
                Mod_SSS_1 = WORK;
                Mod_SSS_2 = FREE;
                Mod_Onix_2 = WORK;

                return;
            }

            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ» СВОБОДЕН
            if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS == BUSY_PHASING && Mod_SSS_fz == NULL  && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = BUSY_PHASING;
                Mod_Onix_2 = CALL;

                return;
            }



            //Режим фазирования СТАТУС ССС «ОТБОЙ»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == HANGUP && Mod_SSS == HANGUP && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = PHASING;
                Mod_Onix_2 = CALL;

                return;
            }

            //Режим фазирования СТАТУС ССС «ЗАНЯТО»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == NULL && Mod_SSS == BUSY && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = BUSY;
                Mod_Onix_2 = CALL;

                return;
            }


            //Режим фазирования СТАТУС ССС «ЗАНЯТО (ФЗ)»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == BUSY && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = PHASING;
                Mod_Onix_2 = CALL;

                return;
            }


            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ (ФЗ)»
            if(Mod_Onix_1 == FREE && Mod_SSS_mod == PHASING && Mod_SSS_fz == BUSY_PHASING && Mod_SSS_1 == FREE && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = PHASING;
                Mod_Onix_2 = CALL;
                return;
            }

            //Режим фазирования РАБОТА
            if(Mod_Onix_1 == FREE && Mod_SSS_1 == RECEIPT && Mod_SSS_mod == PHASING  && Mod_SSS == WORK && Mod_Onix_2 ==FREE)
            {
                Mod_Onix_1 = FREE;
                Mod_SSS_1 = WORK;
                Mod_SSS_2 = WORK;
                Mod_Onix_2 = FREE;

                return;
            }


            //Режим фазирования ВЫЗОВ
            if(Mod_Onix_1 == FREE &&  Mod_SSS_2 == FREE && Mod_SSS_mod == PHASING &&  Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {

                switch (Mod_SSS) {

                    case WORK:
                    {
                        Mod_SSS_1  =  FREE;
                        Mod_SSS_2  =  PHASING;
                        Mod_Onix_2 = CALL;

                        return;
                    }

                }

                Mod_Onix_1 = FREE;
                Mod_SSS_1 = FREE;
                Mod_SSS_2 = PHASING;
                Mod_Onix_2 = CALL;

                return;
            }


            //Квитанция

            //Режим фазирования СТАТУС ССС «ЗАНЯТ ФИЗ ВХ/ВЫХ» ИС (Квитанция)
            if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_mod == RECEIPT  && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {
                Mod_Onix_1 = IS;
                Mod_SSS_1 = IS;
                Mod_SSS_2 = RECEIPT;
                Mod_Onix_2 = CALL;
                return;
            }



              if(Mod_Onix_1 == IS && Mod_SSS_1== IS && Mod_SSS_mod == PHASING && Mod_Onix_2 == FREE)
              {
                  Mod_Onix_1 = IS;
                  Mod_SSS_1 = IS;
                  Mod_SSS_2 = PHASING;
                  Mod_Onix_2 = CALL;

                  return;
              }


             if(Mod_Onix_1 == FREE && Mod_SSS_1 == RECEIPT && Mod_SSS == WORK && Mod_Onix_2 ==FREE)
             {
                 Mod_Onix_1 = FREE;

                 Mod_SSS_1 = WORK;

                 Mod_SSS_2 = WORK;

                 Mod_Onix_2 = FREE;
                 return;
             }


            if(Mod_Onix_1 == RADIO && Mod_SSS_1 == FREE && Mod_SSS_2 == RADIO && Mod_Onix_2 ==FREE)
            {
                Mod_Onix_1 = RADIO;

                Mod_SSS_1 = CALL2;

                Mod_SSS_2 = RADIO;

                Mod_Onix_2 = CALL2;
                return;
            }

             if(Mod_Onix_1 == CALL2 && Mod_SSS_1 == RADIO && Mod_SSS_2 == CALL2 && Mod_Onix_2 ==RADIO)
             {
                 Mod_Onix_1 = NULL;
                 Mod_SSS_1 = PPV;
                 Mod_SSS_2 = NULL;
                 Mod_Onix_2 = PPV;
                 return;
             }

             if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_2 == FREE && Mod_Onix_2 ==FREE)
             {
                 switch (Mod_SSS) {

                     case BUSY_PHASING:
                     {
                         Mod_SSS_2 = BUSY_PHASING;
                         Mod_Onix_2 = CALL;
                         return;
                     }

                 }
             }

            //Вызов со стороны OnixB 2
            if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
            {


                switch (Mod_SSS) {

                    case WORK:
                    {
                        Mod_SSS_1  =  FREE;
                        Mod_SSS_2  =  RECEIPT;

                        return;
                    }

                    case BUSY:
                    {
                        Mod_SSS_2 = BUSY;
                        Mod_Onix_2 = CALL;
                        return;
                    }

                    case HANGUP:
                    {
                        Mod_SSS_2 = HANGUP;
                        Mod_Onix_2 = CALL;
                        return;
                    }

                    case BUSY_PHASING:
                    {
                        Mod_SSS_2 = BUSY_PHASING;
                        Mod_Onix_2 = CALL;
                        return;
                    }

                }

                Mod_Onix_2 = CALL;

                Mod_SSS_2 = RECEIPT;

                Mod_SSS_1 = CALL;

                return;


            }

            // Работа к OnixB 1
            if(Mod_Onix_1 == CALL && Mod_SSS_1 == RECEIPT && Mod_SSS_2 == CALL && Mod_Onix_2 ==WORK)
            {
                Mod_Onix_2 = WORK;

                Mod_SSS_2 = FREE;

                Mod_SSS_1 = WORK;

            }

        }
    }
}

void Sss::startSimulation(int time)
{
    timer->start(time);
}

void Sss::stopSimulation()
{
    timer->stop();
}

void Sss::clearAll()
{
    Mod_Onix_1 = FREE;
    Mod_SSS_1 = FREE;
    Mod_Onix_2 = FREE;
    Mod_SSS_2 = FREE;

    Mod_SSS = NULL;
    Mod_SSS_fz = NULL;


    Number_1.clear();
    Number_2.clear();

    setNumber = false;
    setNumber_1 = false;
    count = 0;
    kol = 0;
    kol2 = 0;
}

void Sss::free()
{
    if(Mod_Onix_1 == FREE && Mod_Onix_2 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == FREE)
    {

            emit activ();
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == RECEIPT && Mod_SSS_2 == FREE && Mod_Onix_2 ==FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>КВИТАНЦИЯ</b>\n");
        qDebug () << "1) SSS отправил команду КВИТАНЦИЯ.";

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(RECEIPT);
        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == RECEIPT && Mod_Onix_2 ==FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>КВИТАНЦИЯ</b>\n");
        qDebug () << "2) SSS отправил команду КВИТАНЦИЯ.";

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(FREE);
        emit writeSSS2(RECEIPT);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == FREE && Mod_Onix_2 ==FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b>\n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(FREE);
        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == CALL && Mod_SSS_2 == RECEIPT && Mod_Onix_2 ==CALL)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>КВИТАНЦИЯ</b>.\n");
        qDebug () << "2) SSS отправил команду КВИТАНЦИЯ.";

     //   emit log1("1) SSS отправил команду <b>ВЫЗОВ</b> \n Номер: <b>" + QString::number(Number_2.value(0),16) +"\n" + QString::number(Number_2.value(1),16) +"\n" + QString::number(Number_2.value(2),16)+"\n" + QString::number(Number_2.value(3),16) +"</b>\n");

         emit log1("1) SSS отправил команду <b>ВЫЗОВ</b> \n Номер: <b>" + nomer(Number_2)+"</b>\n");

        qDebug () << "1) SSS отправил команду ВЫЗОВ.";

        emit writeSSS2(RECEIPT);

        emit writeSSS(CALL);

        emit writeSSS(Number_2.value(0));
        emit writeSSS(Number_2.value(1));
        emit writeSSS(Number_2.value(2));
        emit writeSSS(Number_2.value(3));
         return;
    }


    if(Mod_Onix_1 == CALL && Mod_SSS_1 == RECEIPT && Mod_SSS_2 == CALL && Mod_Onix_2 ==FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>КВИТАНЦИЯ</b>.\n");
        qDebug () << "1) SSS отправил команду КВИТАНЦИЯ.";

      //  emit log2("2) SSS отправил команду <b>ВЫЗОВ</b> \n Номер: <b>" + QString::number(Number_1.value(0),16) +"\n" + QString::number(Number_1.value(1),16) +"\n" + QString::number(Number_1.value(2),16)+"\n" + QString::number(Number_1.value(3),16) +"</b>\n");

        emit log2("2) SSS отправил команду <b>ВЫЗОВ</b> \n Номер: <b>" + nomer(Number_1)+"</b>\n");

        qDebug () << "2) SSS отправил команду ВЫЗОВ.";

        emit writeSSS(RECEIPT);

        emit writeSSS2(CALL);

        emit writeSSS2(Number_1.value(0));
        emit writeSSS2(Number_1.value(1));
        emit writeSSS2(Number_1.value(2));
        emit writeSSS2(Number_1.value(3));
         return;
    }

    if(Mod_Onix_1 == WORK && Mod_SSS_1 == FREE && Mod_SSS_2 == WORK && Mod_Onix_2 == WORK)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

       // emit log2("2) SSS отправил команду <b>РАБОТА</b>.\n Номер: <b>" + QString::number(Number_1.value(0),16) +"\n" + QString::number(Number_1.value(1),16) +"\n" + QString::number(Number_1.value(2),16)+"\n" + QString::number(Number_1.value(3),16) +"</b>\n");

        emit log2("2) SSS отправил команду <b>РАБОТА</b> \n Номер: <b>" + nomer(Number_1)+"</b>\n");

        qDebug () << "2) SSS отправил команду РАБОТА.";

        emit log1("1) SSS отправил команду <b>СВОБОДЕН.</b> \n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(FREE);

        emit writeSSS2(WORK);

        emit writeSSS2(Number_1.value(0));
        emit writeSSS2(Number_1.value(1));
        emit writeSSS2(Number_1.value(2));
        emit writeSSS2(Number_1.value(3));
         return;
    }

    if(Mod_Onix_1 == WORK && Mod_SSS_1 == WORK && Mod_SSS_2 == FREE && Mod_Onix_2 == WORK)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

       // emit log1("1) SSS отправил команду <b>РАБОТА</b>.\n Номер: <b>" + QString::number(Number_2.value(0),16) +"\n" + QString::number(Number_2.value(1),16) +"\n" + QString::number(Number_2.value(2),16)+"\n" + QString::number(Number_2.value(3),16) +"</b>\n");

        emit log1("1) SSS отправил команду <b>РАБОТА</b>.\n Номер: <b>" + nomer(Number_2) +"</b>\n");

        qDebug () << "1) SSS отправил команду РАБОТА.";

        emit log2("2) SSS отправил команду <b>СВОБОДЕН.</b> \n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit writeSSS2(FREE);

        emit writeSSS(WORK);

        emit writeSSS(Number_2.value(0));
        emit writeSSS(Number_2.value(1));
        emit writeSSS(Number_2.value(2));
        emit writeSSS(Number_2.value(3));
         return;

    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == WORK && Mod_SSS_2 == WORK && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

       // emit log1("1) SSS отправил команду <b>РАБОТА</b>.\n Номер: <b>" + QString::number(Number_1.value(0),16) +"\n" + QString::number(Number_1.value(1),16) +"\n" + QString::number(Number_1.value(2),16)+"\n" + QString::number(Number_1.value(3),16) +"</b>\n");

        emit log1("1) SSS отправил команду <b>РАБОТА</b>.\n Номер: <b>" + nomer(Number_1) +"</b>\n");

        qDebug () << "1) SSS отправил команду РАБОТА.";

       // emit log2("2) SSS отправил команду <b>РАБОТА</b>.\n Номер: <b>" + QString::number(Number_2.value(0),16) +"\n" + QString::number(Number_2.value(1),16) +"\n" + QString::number(Number_2.value(2),16)+"\n" + QString::number(Number_2.value(3),16) +"</b>\n");

        emit log2("2) SSS отправил команду <b>РАБОТА</b>.\n Номер: <b>" + nomer(Number_2) +"</b>\n");

        qDebug () << "2) SSS отправил команду РАБОТА.";

        emit writeSSS2(WORK);

        emit writeSSS2(Number_2.value(0));
        emit writeSSS2(Number_2.value(1));
        emit writeSSS2(Number_2.value(2));
        emit writeSSS2(Number_2.value(3));

        emit writeSSS(WORK);

        emit writeSSS(Number_1.value(0));
        emit writeSSS(Number_1.value(1));
        emit writeSSS(Number_1.value(2));
        emit writeSSS(Number_1.value(3));

        return;

    }

    if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ИС<b>.\n");
        qDebug () << "2) SSS отправил команду ИС.";

        emit log1("1) SSS отправил команду <b>ИС</b> \n");
        qDebug () << "1) SSS отправил команду ИС.";

        emit writeSSS(IS);

        emit writeSSS2(IS);


        switch (Mod_SSS) {
            case BUSY:
            {
                Mod_SSS_1  =  BUSY;
                Mod_SSS_2  =  BUSY;

                break;
            }

            case PRIORITY_CALL_OFF:
            {
                Mod_SSS_1  =  PRIORITY_CALL_OFF;
                Mod_SSS_2  =  PRIORITY_CALL_OFF;
                break;
            }
        }

         return;

    }

    if(Mod_Onix_1 == HANGUP && Mod_SSS_1 == HANGUP && Mod_SSS_2 == HANGUP && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ОТБОЙ<b>.\n");
        qDebug () << "2) SSS отправил команду ОТБОЙ.";

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b> \n");
        qDebug () << "1) SSS отправил команду ОТБОЙ.";

        emit writeSSS(HANGUP);

        emit writeSSS2(HANGUP);
         return;
    }

    if(Mod_Onix_1 == HANGUP && Mod_SSS_1 == HANGUP && Mod_SSS_2 == HANGUP && Mod_Onix_2 == WORK)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ОТБОЙ<b>.\n");
        qDebug () << "2) SSS отправил команду ОТБОЙ.";

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b> \n");
        qDebug () << "1) SSS отправил команду ОТБОЙ.";

        emit writeSSS(HANGUP);

        emit writeSSS2(HANGUP);
         return;
    }

    // HUNGUP    ss1 = HUNGUP  ss2 = FREE  onix2 = FREE
    if(Mod_Onix_1 == FREE && Mod_SSS_1 == HANGUP && Mod_SSS_2 == HANGUP && Mod_Onix_2 == HANGUP)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ОТБОЙ</b>.\n");
        qDebug () << "2) SSS отправил команду ОТБОЙ.";

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b> \n");
        qDebug () << "1) SSS отправил команду ОТБОЙ.";

        emit writeSSS(HANGUP);

        emit writeSSS2(HANGUP);
         return;
    }

    if(Mod_Onix_1 == WORK && Mod_SSS_1 == HANGUP && Mod_SSS_2 == HANGUP && Mod_Onix_2 == HANGUP)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ОТБОЙ<b>.\n");
        qDebug () << "2) SSS отправил команду ОТБОЙ.";

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b> \n");
        qDebug () << "1) SSS отправил команду ОТБОЙ.";

        emit writeSSS(HANGUP);

        emit writeSSS2(HANGUP);
         return;
    }

    if(Mod_Onix_1 == HANGUP && Mod_SSS_1 == HANGUP && Mod_SSS_2 == HANGUP && Mod_Onix_2 == IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ОТБОЙ</b>.\n");
        qDebug () << "2) SSS отправил команду ОТБОЙ.";

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b> \n");
        qDebug () << "1) SSS отправил команду ОТБОЙ.";

        emit writeSSS(HANGUP);

        emit writeSSS2(HANGUP);
         return;
    }

    // 1 = IS  SS1 = HANGUP SS2 = FREE 2 = FREE
    if(Mod_Onix_1 == IS && Mod_SSS_1 == HANGUP && Mod_SSS_2 == HANGUP && Mod_Onix_2 == HANGUP)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ОТБОЙ</b>.\n");
        qDebug () << "2) SSS отправил команду ОТБОЙ.";

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b> \n");
        qDebug () << "1) SSS отправил команду ОТБОЙ.";

        emit writeSSS(HANGUP);

        emit writeSSS2(HANGUP);

        return;
    }

    if(Mod_Onix_1 == BUSY && Mod_SSS_1 == FREE && Mod_SSS_2 == BUSY && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ЗАНЯТО</b>.\n");
        qDebug () << "2) SSS отправил команду ЗАНЯТО.";

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(FREE);

        emit writeSSS2(BUSY);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == BUSY && Mod_SSS_2 == FREE  && Mod_Onix_2 == BUSY )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ЗАНЯТО</b>.\n");
        qDebug () << "1) SSS отправил команду ЗАНЯТО.";

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit writeSSS2(FREE);

        emit writeSSS(BUSY);

        return;
    }

    if(Mod_Onix_1 == IS && Mod_SSS_1 == BUSY && Mod_SSS_2 == BUSY && Mod_Onix_2 == IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ЗАНЯТО</b>.\n");
        qDebug () << "2) SSS отправил команду ЗАНЯТО.";

        emit log1("1) SSS отправил команду <b>ЗАНЯТО</b> \n");
        qDebug () << "1) SSS отправил команду ЗАНЯТО.";

        emit writeSSS(BUSY);

        emit writeSSS2(BUSY);

        return;
    }

    if(Mod_Onix_1 == IS && Mod_SSS_1 == PRIORITY_CALL_OFF && Mod_SSS_2 == PRIORITY_CALL_OFF && Mod_Onix_2 == IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ОТБОЙ ПО ПРИОРИТЕТУ</b>.\n");
        qDebug () << "2) SSS отправил команду ОТБОЙ ПО ПРИОРИТЕТУ.";

        emit log1("1) SSS отправил команду <b>ОТБОЙ ПО ПРИОРИТЕТУ</b> \n");
        qDebug () << "1) SSS отправил команду ОТБОЙ ПО ПРИОРИТЕТУ.";

        emit writeSSS(PRIORITY_CALL_OFF);

        emit writeSSS2(PRIORITY_CALL_OFF);

        return;
    }


    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == BUSY && Mod_Onix_2 == CALL)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ЗАНЯТО</b>.\n");
        qDebug () << "2) SSS отправил команду ЗАНЯТО.";

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(FREE);

        emit writeSSS2(BUSY);

        return;
    }

    if(Mod_Onix_1 == CALL && Mod_SSS_1 == BUSY && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit log1("1) SSS отправил команду <b>ЗАНЯТО</b> \n");
        qDebug () << "1) SSS отправил команду ЗАНЯТО.";

        emit writeSSS(BUSY);

        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == HANGUP && Mod_Onix_2 == CALL)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ОТБОЙ</b>.\n");
        qDebug () << "2) SSS отправил команду ОТБОЙ.";

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(FREE);

        emit writeSSS2(HANGUP);

        return;
    }

    if(Mod_Onix_1 == CALL && Mod_SSS_1 == HANGUP && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b> \n");
        qDebug () << "1) SSS отправил команду ОТБОЙ.";

        emit writeSSS(HANGUP);

        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_2 == BUSY_PHASING && Mod_Onix_2 == CALL)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ИС</b>.\n");
        qDebug () << "1) SSS отправил команду ИС.";

        emit log2("2) SSS отправил команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b> \n");
        qDebug () << "2) SSS отправил команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";

        emit writeSSS(IS);

        emit writeSSS2(BUSY_PHASING);

        return;
    }

    if(Mod_Onix_1 == CALL && Mod_SSS_1 == BUSY_PHASING && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ИС</b>.\n");
        qDebug () << "2) SSS отправил команду ИС.";

        emit log1("1) SSS отправил команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b> \n");
        qDebug () << "1) SSS отправил команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";

        emit writeSSS(BUSY_PHASING);

        emit writeSSS2(IS);

        return;
    }

    if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ИС<b>.\n");
        qDebug () << "1) SSS отправил команду ИС.";

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(IS);

        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ИС<b>.\n");
        qDebug () << "2) SSS отправил команду ИС.";

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit writeSSS2(IS);

        emit writeSSS(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == BUSY_PHASING && Mod_Onix_2 == CALL)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>СВОБОДЕН<b>.\n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit log2("2) SSS отправил команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b> \n");
        qDebug () << "2) SSS отправил команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";

        emit writeSSS(FREE);

        emit writeSSS2(BUSY_PHASING);

        return;
    }

    if(Mod_Onix_1 == CALL && Mod_SSS_1 == BUSY_PHASING && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>СВОБОДЕН<b>.\n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit log1("1) SSS отправил команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b> \n");
        qDebug () << "1) SSS отправил команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";

        emit writeSSS(BUSY_PHASING);

        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == RADIO && Mod_SSS_2 == FREE && Mod_Onix_2 == RADIO)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

      //  kol2++;
      //  emit log2("-----");
      //  emit log2("- "+QString::number(kol2)+" -");

      //  emit log2("2) SSS отправил команду <b>СВОБОДЕН<b>.\n");
       // qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit log1("1) SSS отправил команду <b>РАДИО</b> \n");
        qDebug () << "1) SSS отправил команду РАДИО.";

        emit writeSSS(RADIO);

      //  emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == RADIO && Mod_SSS_1 == FREE && Mod_SSS_2 == RADIO && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>РАДИО<b>.\n");
        qDebug () << "2) SSS отправил команду РАДИО.";

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(FREE);

        emit writeSSS2(RADIO);

        return;
    }

    if(Mod_Onix_1 == CALL2 && Mod_SSS_1 == RADIO && Mod_SSS_2 == CALL2 && Mod_Onix_2 == RADIO)
    {
       // kol++;
       // emit log1("-----");
      //  emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ВЫЗОВ2</b>.\n Номер: <b>" + nomer(Number_1) +"</b>\n");
        qDebug () << "2) SSS отправил команду ВЫЗОВ2.";

       // emit log1("1) SSS отправил команду <b>РАДИО</b> \n");
       // qDebug () << "1) SSS отправил команду РАДИО.";

       // emit writeSSS(RADIO);

        emit writeSSS2(CALL2);

        emit writeSSS2(Number_1.value(0));
        emit writeSSS2(Number_1.value(1));
        emit writeSSS2(Number_1.value(2));
        emit writeSSS2(Number_1.value(3));

        return;
    }

    if(Mod_Onix_1 == RADIO && Mod_SSS_1 == CALL2 && Mod_SSS_2 == RADIO && Mod_Onix_2 == CALL2)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ВЫЗОВ2</b>.\n Номер: <b>" + nomer(Number_2) +"</b>\n");
        qDebug () << "1) SSS отправил команду ВЫЗОВ2.";

        emit log2("2) SSS отправил команду <b>РАДИО</b> \n");
        qDebug () << "2) SSS отправил команду РАДИО.";

        emit writeSSS2(RADIO);

        emit writeSSS(CALL2);

        emit writeSSS(Number_2.value(0));
        emit writeSSS(Number_2.value(1));
        emit writeSSS(Number_2.value(2));
        emit writeSSS(Number_2.value(3));

        return;
    }

    if(Mod_Onix_1 == NULL && Mod_SSS_1 == PPV && Mod_SSS_2 == NULL && Mod_Onix_2 == PPV)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

      //  kol2++;
      //  emit log2("-----");
       // emit log2("- "+QString::number(kol2)+" -");

      //  emit log2("2) SSS отправил команду <b>ВЫЗОВ2</b>.\n");
      //  qDebug () << "2) SSS отправил команду ВЫЗОВ2.";

        emit log1("1) SSS отправил команду <b>ППВ</b>\n Номер: <b>" + nomer(Number_2) +"</b>\n");
        qDebug () << "1) SSS отправил команду ППВ.";

        emit writeSSS(PPV);

        emit writeSSS(Number_2.value(0));
        emit writeSSS(Number_2.value(1));
        emit writeSSS(Number_2.value(2));
        emit writeSSS(Number_2.value(3));

      //  emit writeSSS2(CALL2);

      //  emit writeSSS2(Number_1.value(0));
      //  emit writeSSS2(Number_1.value(1));
      //  emit writeSSS2(Number_1.value(2));
      //  emit writeSSS2(Number_1.value(3));

        return;
    }


    if(Mod_Onix_1 == PPV && Mod_SSS_1 == NULL && Mod_SSS_2 == PPV && Mod_Onix_2 == NULL)
    {
       // kol++;
       // emit log1("-----");
       // emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

       // emit log1("1) SSS отправил команду <b>ВЫЗОВ2</b>.\n");
       // qDebug () << "1) SSS отправил команду ВЫЗОВ2.";

        emit log2("2) SSS отправил команду <b>ППВ</b>\n Номер: <b>" + nomer(Number_1) +"</b>\n");
        qDebug () << "2) SSS отправил команду ППВ.";

        emit writeSSS2(PPV);

        emit writeSSS2(Number_1.value(0));
        emit writeSSS2(Number_1.value(1));
        emit writeSSS2(Number_1.value(2));
        emit writeSSS2(Number_1.value(3));

      //  emit writeSSS(CALL2);

       // emit writeSSS(Number_2.value(0));
       // emit writeSSS(Number_2.value(1));
       // emit writeSSS(Number_2.value(2));
       // emit writeSSS(Number_2.value(3));

        return;
    }

    if(Mod_Onix_1 == CALL2Replay && Mod_SSS_1 == PPV && Mod_SSS_2 == CALL2Replay && Mod_Onix_2 == PPV)
    {
      //  kol++;
       // emit log1("-----");
       // emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ППО</b>.\n");
        qDebug () << "2) SSS отправил команду ППО.";

       // emit log1("1) SSS отправил команду <b>ППВ</b> \n");
      //  qDebug () << "1) SSS отправил команду ППВ.";

      //  emit writeSSS(PPV);

       // emit writeSSS(Number_2.value(0));
      //  emit writeSSS(Number_2.value(1));
      //  emit writeSSS(Number_2.value(2));
      //  emit writeSSS(Number_2.value(3));

        emit writeSSS2(CALL2Replay);

        return;
    }

    if(Mod_Onix_1 == PPV && Mod_SSS_1 == CALL2Replay && Mod_SSS_2 == PPV && Mod_Onix_2 == CALL2Replay)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ППО</b>.\n");
        qDebug () << "1) SSS отправил команду ППО.";

        emit log2("2) SSS отправил команду <b>ППВ</b>\n Номер: <b>" + nomer(Number_1) +"</b>\n");
        qDebug () << "2) SSS отправил команду ППВ.";

        emit writeSSS(CALL2Replay);

        emit writeSSS2(PPV);

        emit writeSSS2(Number_1.value(0));
        emit writeSSS2(Number_1.value(1));
        emit writeSSS2(Number_1.value(2));
        emit writeSSS2(Number_1.value(3));



        return;
    }

    if(Mod_Onix_1 == CALL && Mod_SSS_1 == PHASING && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ФАЗИРОВАНИЕ</b>.\n");
        qDebug () << "1) SSS отправил команду ФАЗИРОВАНИЕ.";

        emit log2("2) SSS отправил команду <b>ИС</b> \n");
        qDebug () << "2) SSS отправил команду ИС.";

        emit writeSSS(PHASING);

        emit writeSSS2(IS);


        return;
    }

    if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_2 == PHASING  && Mod_Onix_2 == CALL)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ИС</b>.\n");
        qDebug () << "1) SSS отправил команду ИС.";

        emit log2("2) SSS отправил команду <b>ФАЗИРОВАНИЕ</b> \n");
        qDebug () << "2) SSS отправил команду ФАЗИРОВАНИЕ.";

        emit writeSSS(IS);

        emit writeSSS2(PHASING);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == RECEIPT && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>КВИТАНЦИЯ</b>.\n");
        qDebug () << "1) SSS отправил команду КВИТАНЦИЯ.";

        emit log2("2) SSS отправил команду <b>ИС</b> \n");
        qDebug () << "2) SSS отправил команду ИС.";

        emit writeSSS2(IS);

        emit writeSSS(RECEIPT);

        return;
    }

    if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_2 == RECEIPT && Mod_Onix_2 == FREE )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ИС</b>.\n");
        qDebug () << "1) SSS отправил команду ИС.";

        emit log2("2) SSS отправил команду <b>КВИТАНЦИЯ</b> \n");
        qDebug () << "2) SSS отправил команду КВИТАНЦИЯ.";

        emit writeSSS(IS);

        emit writeSSS2(RECEIPT);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == BUSY_PHASING && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ИС</b>.\n");
        qDebug () << "2) SSS отправил команду ИС.";

        emit log1("1) SSS отправил команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b> \n");
        qDebug () << "1) SSS отправил команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";

        emit writeSSS(BUSY_PHASING);

        emit writeSSS2(IS);

        return;
    }

    if(Mod_Onix_1 == IS && Mod_SSS_1 == IS && Mod_SSS_2 == BUSY_PHASING && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ИС</b>.\n");
        qDebug () << "1) SSS отправил команду ИС.";

        emit log2("2) SSS отправил команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b> \n");
        qDebug () << "2) SSS отправил команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";

        emit writeSSS(IS);

        emit writeSSS2(BUSY_PHASING);

        return;
    }

    if(Mod_Onix_1 == CALL && Mod_SSS_1 == PHASING  && Mod_SSS == BUSY_PHASING   && Mod_SSS_mod == PHASING && Mod_SSS_fz == NULL && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit log1("1) SSS отправил команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b> \n");
        qDebug () << "1) SSS отправил команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";

        emit writeSSS(BUSY_PHASING);

        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE  && Mod_SSS == BUSY_PHASING   && Mod_SSS_mod == PHASING && Mod_SSS_fz == NULL && Mod_SSS_2 ==  PHASING && Mod_Onix_2 == CALL )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit log2("2) SSS отправил команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b> \n");
        qDebug () << "2) SSS отправил команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";

        emit writeSSS2(BUSY_PHASING);

        emit writeSSS(FREE);

        return;
    }

    if(Mod_Onix_1 == CALL && Mod_SSS_1 == PHASING && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit log1("1) SSS отправил команду <b>ФАЗИРОВАНИЕ</b> \n");
        qDebug () << "1) SSS отправил команду ФАЗИРОВАНИЕ.";

        emit writeSSS(PHASING);

        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == PHASING && Mod_Onix_2 == CALL)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit log2("2) SSS отправил команду <b>ФАЗИРОВАНИЕ</b> \n");
        qDebug () << "2) SSS отправил команду ФАЗИРОВАНИЕ.";

        emit writeSSS(FREE);

        emit writeSSS2(PHASING);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == BUSY_PHASING && Mod_SSS_2 == FREE && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit log1("1) SSS отправил команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b> \n");
        qDebug () << "1) SSS отправил команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";

        emit writeSSS2(FREE);

        emit writeSSS(BUSY_PHASING);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE && Mod_SSS_2 == BUSY_PHASING && Mod_Onix_2 == FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b>.\n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit log2("2) SSS отправил команду <b>ЗАНЯТ ФИЗ ВХ/ВЫХ</b> \n");
        qDebug () << "2) SSS отправил команду ЗАНЯТ ФИЗ ВХ/ВЫХ.";

        emit writeSSS(FREE);

        emit writeSSS2(BUSY_PHASING);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == BUSY  && Mod_SSS_2 == FREE  && Mod_Onix_1 == FREE )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ЗАНЯТО</b>.\n");
        qDebug () << "1) SSS отправил команду ЗАНЯТО.";

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(BUSY);

        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE  && Mod_SSS_2 == BUSY  && Mod_Onix_1 == FREE )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ЗАНЯТО</b>.\n");
        qDebug () << "2) SSS отправил команду ЗАНЯТО.";

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit writeSSS2(BUSY);

        emit writeSSS(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE  && Mod_SSS_2 == HANGUP  && Mod_Onix_1 == FREE )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ОТБОЙ</b>.\n");
        qDebug () << "2) SSS отправил команду ОТБОЙ.";

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit writeSSS2(HANGUP);

        emit writeSSS(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == HANGUP  && Mod_SSS_2 == FREE  && Mod_Onix_1 == FREE )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b>.\n");
        qDebug () << "1) SSS отправил команду ОТБОЙ.";

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(HANGUP);

        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == HANGUP && Mod_SSS_1 == HANGUP  && Mod_SSS_2 == FREE  && Mod_Onix_2 == FREE )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b>.\n");
        qDebug () << "1) SSS отправил команду ОТБОЙ.";

        emit log2("2) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "2) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(HANGUP);

        emit writeSSS2(FREE);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == FREE  && Mod_SSS_2 == HANGUP  && Mod_Onix_2 == HANGUP )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>ОТБОЙ</b>.\n");
        qDebug () << "2) SSS отправил команду ОТБОЙ.";

        emit log1("1) SSS отправил команду <b>СВОБОДЕН</b> \n");
        qDebug () << "1) SSS отправил команду СВОБОДЕН.";

        emit writeSSS(FREE);

        emit writeSSS2(HANGUP);

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == CALL  && Mod_SSS_2 == RECEIPT  && Mod_Onix_2 == FREE )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>КВИТАНЦИЯ</b>.\n");
        qDebug () << "2) SSS отправил команду КВИТАНЦИЯ.";

      //  emit log1("1) SSS отправил команду <b>ВЫЗОВ</b> \n Номер: <b>" + QString::number(Number_2.value(0),16) +"\n" + QString::number(Number_2.value(1),16) +"\n" + QString::number(Number_2.value(2),16)+"\n" + QString::number(Number_2.value(3),16) +"</b>\n");

        emit log1("1) SSS отправил команду <b>ВЫЗОВ</b>.\n Номер: <b>" + nomer(Number_2) +"</b>\n");

        qDebug () << "1) SSS отправил команду ВЫЗОВ.";

        emit writeSSS2(RECEIPT);

        emit writeSSS(CALL);

        emit writeSSS(Number_2.value(0));
        emit writeSSS(Number_2.value(1));
        emit writeSSS(Number_2.value(2));
        emit writeSSS(Number_2.value(3));

        return;
    }

    if(Mod_Onix_1 == FREE && Mod_SSS_1 == RECEIPT  && Mod_SSS_2 == CALL   && Mod_Onix_2 == FREE )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>КВИТАНЦИЯ</b>.\n");
        qDebug () << "1) SSS отправил команду КВИТАНЦИЯ.";

      //  emit log2("2) SSS отправил команду <b>ВЫЗОВ</b> \n Номер: <b>" + QString::number(Number_1.value(0),16) +"\n" + QString::number(Number_1.value(1),16) +"\n" + QString::number(Number_1.value(2),16)+"\n" + QString::number(Number_1.value(3),16) +"</b>\n");


        emit log2("2) SSS отправил команду <b>ВЫЗОВ</b> \n Номер: <b>" + nomer(Number_1) +"</b>\n");

        qDebug () << "2) SSS отправил команду ВЫЗОВ.";

        emit writeSSS(RECEIPT);

        emit writeSSS2(CALL);

        emit writeSSS2(Number_1.value(0));
        emit writeSSS2(Number_1.value(1));
        emit writeSSS2(Number_1.value(2));
        emit writeSSS2(Number_1.value(3));

        return;
    }

    //Квитанция Занят физ/выход
    if(Mod_Onix_1 == CALL && Mod_SSS_1 == RECEIPT && Mod_SSS_mod == RECEIPT  && Mod_SSS_2 == IS && Mod_Onix_2 == IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log1("1) SSS отправил команду <b>КВИТАНЦИЯ</b>.\n");
        qDebug () << "1) SSS отправил команду КВИТАНЦИЯ.";

        emit log2("2) SSS отправил команду <b>ИС</b>.\n");
        qDebug () << "2) SSS отправил команду ИС.";

        emit writeSSS(RECEIPT);

        emit writeSSS2(IS);

    }

    //Квитанция Занят физ/выход
    if(Mod_Onix_1 == IS  && Mod_SSS_1 == IS && Mod_SSS_mod == RECEIPT  && Mod_SSS_2 == RECEIPT && Mod_Onix_2 == CALL)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        emit log2("2) SSS отправил команду <b>КВИТАНЦИЯ</b>.\n");
        qDebug () << "2) SSS отправил команду КВИТАНЦИЯ.";

        emit log1("1) SSS отправил команду <b>ИС</b>.\n");
        qDebug () << "1) SSS отправил команду ИС.";

        emit writeSSS2(RECEIPT);

        emit writeSSS(IS);

    }



}

void Sss::setMod_SSS(quint16 data)
{
    Mod_SSS = data;
}

void Sss::setMod_SSS_fz(quint16 data)
{
    Mod_SSS_fz = data;
}

void Sss::setMod_SSS_mod(quint16 data)
{
      Mod_SSS_mod = data;
}

void Sss::setMod_IS()
{
    Mod_Onix_1 = IS;
    Mod_SSS_1 = IS;
    Mod_SSS_2 = IS;
    Mod_Onix_2 = IS;
}

void Sss::setMod_IS_1()
{
    Mod_Onix_1 = IS;
    Mod_SSS_1 = IS;
    Mod_SSS_2 = FREE;
    Mod_Onix_2 = FREE;
}

void Sss::setMod_IS_2()
{
    Mod_Onix_1 = FREE;
    Mod_SSS_1 = FREE;
    Mod_SSS_2 = IS;
    Mod_Onix_2 = IS;
}

QString Sss::nomer(QList<quint16> number)
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
        default: list +=  "ПОВТОР";//list[list.count()-1];break;

        }
    }




    return list;
}

