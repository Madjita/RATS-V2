#ifndef ONIXB_H
#define ONIXB_H

#include <QObject>

#include <QDebug>
#include <QThread>

#include <QTimer>
#include <QTime>

//ISS 3
//«ВЫЗОВ», «ОТВЕТ РТР», «ОТВЕТ ЗС», «ОТБОЙ ЗС», «ОТБОЙ ПО ПРИОРИТЕТУ», «ЗАНЯТА ЗС»
#pragma pack(push, 1)
struct ISS3_Call
{
    quint8 comand : 4 ; //(4 бита) Код команды
    quint8 nbp : 2; // (2 бита) Номер базовой последователности
    quint8 speed : 2; // (2 бита) Информационная скорость абонентского сигнала.

    // (16 бит) Собственный номер абонента в двоичном коде
    quint8 numberSendMSB;
    quint8 numberSendLSB;

    // (16 бит) Номер вызываемого абонента в двоичном коде
    quint8 numberReceiverMSB;
    quint8 numberReceiverLSB;


};
#pragma pack(pop)


//«РЕГИСТРАЦИЯ», «ПОДТВЕРЖДЕНИЕ РЕГИСТРАЦИИ», «ОТКАЗ ОТ РЕГИСТРАЦИИ», «ПОДТВЕРЖДЕНИЕ ОТКАЗА ОТ РЕГИСТРАЦИИ»
#pragma pack(push, 1)
struct ISS3_Receipt
{
    quint8 comand : 4 ; //(4 бита) Код команды
    quint8 nbp : 2; // (2 бита) Номер базовой последователности
    quint8 speed : 2; // (2 бита) Информационная скорость абонентского сигнала.

    // (16 бит) Собственный номер абонента в двоичном коде
    quint8 numberSendMSB;
    quint8 numberSendLSB;


    quint8 RChVK : 2; //(2 бита) Тип рабочего ЧВК
    quint8 Rezerv1 : 6; //(6 бита) Заполняется нулями
    quint8 Priopy : 4; //(4 бита) Код уровня приоритета
    quint8 Rezerv2 : 4; //(4 бита) Заполняется нулями


};
#pragma pack(pop)

//Форамт команды «ОТКАЗ РТР»
#pragma pack(push, 1)
struct ISS3_Refusal
{
    quint8 comand : 4 ; //(4 бита) Код команды
    quint8 why : 4; // (2 бита) Код причины отказа

    // (16 бит) Собственный номер абонента в двоичном коде
    quint8 numberSendMSB;
    quint8 numberSendLSB;


    // (16 бит) Номер вызываемого абонента в двоичном коде
    quint8 numberReceiverMSB;
    quint8 numberReceiverLSB;


};
#pragma pack(pop)

//Форамт команды «СВОБОДЕН»
#pragma pack(push, 1)
struct ISS3_Free
{
    quint8 comand : 4 ; //(4 бита) Код команды
    quint8 nbp : 2; // (2 бита) Номер базовой последовательности
    quint8 speed : 2; // (2 бита) Информационная скорость абонентского сигнала.

    // (16 бит) Собственный номер абонента в двоичном коде
    quint8 numberSendMSB;
    quint8 numberSendLSB;


    // (16 бит) Заполняется нулями
    quint8 Rezerv1;
    quint8 Rezerv2;


};
#pragma pack(pop)






class OnixB : public QObject
{
    Q_OBJECT
public:
    explicit OnixB(QObject *parent = nullptr);


    enum STATE
    {
        FREE = 0x183,    //Команда "Свободен" 0x83 = 1000 0011
        CALL = 0x929,    //Команда "Вызов" 0x29 = 0010 1001
        WORK = 0x929,    // Команда "Работа" 0x29 = 0010 1001
        PHASING = 0x51,     //Команда "Фазирование" 0x51 = 0101 0001
        PRIORITY_CALL_OFF = 0x607, // Команда "Отбой по приоритету" 0x29 = 0000 0111
        HANGUP = 0x645,  //Команда "Отбой" 0x45 = 0100 0101
        BUSY = 0x643,    // Команда "Занято" 0x43 = 0100 0011
        BUSY_PHASING = 0x613, // Команда "ЗАНЯТ ФИЗ.ВХ/ВЫХ" 0x13 = 0001 0011
        RECEIPT = 0x23,     //Команда "Квитанция" 0x23 = 0010 0011
        CALL2 = 0x915,       // Команда "Вызов 2" (ППВ) 0x15 = 0001 0101
        PPV = 0x915,       // Команда "Вызов 2" (ППВ) 0x15 = 0001 0101

        IS = 0xFFF, // ИС


        CALL2Replay = 0x661, // Команда "ПОДТВЕРЖДЕНИЕ  ПРИЕМА ОТВ."  0x61 = 0110 0001
        RADIO = 0x189,   // Команда "Радио"  0x89 = 1000 1001

        READY = 0x91,   // Команда "Готов" 0x91 = 1001 0001

        PRIORITY_CALL =0xC1,  // Команда "Вызов приоритетный" 0xС1 = 1100 0001


        REPEAT_1 = 0xBD0,  // Команда "Повтор" 0xD0 = 1101 0000
        REPEAT_2 = 0xCD0,  // Команда "Повтор" 0xD0 = 1101 0000
        REPEAT_3 = 0xDD0,  // Команда "Повтор" 0xD0 = 1101 0000


        // Команда "0" 0x58 = 0101 1000
        ZERO_1 = 0xA58,
        ZERO_2 = 0xB58,
        ZERO_3 = 0xC58,
        ZERO_4 = 0xD58,

        // Команда "1" 0x8C = 1000 1100
        ONE_1 = 0xA8C,
        ONE_2 = 0xB8C,
        ONE_3 = 0xC8C,
        ONE_4 = 0xD8C,

        // Команда "2" 0x4C = 0100 1100
        TWO_1 = 0xA4C,
        TWO_2 = 0xB4C,
        TWO_3 = 0xC4C,
        TWO_4 = 0xD4C,

        // Команда "3" 0xC8 = 1100 1000
        THREE_1 = 0xAC8,
        THREE_2 = 0xBC8,
        THREE_3 = 0xCC8,
        THREE_4 = 0xDC8,

        // Команда "4" 0x2C = 0010 1100
        FOUR_1 = 0xA2C,
        FOUR_2 = 0xB2C,
        FOUR_3 = 0xC2C,
        FOUR_4 = 0xD2C,

        // Команда "5" 0xA8 = 1010 1000
        FIVE_1 = 0xAA8,
        FIVE_2 = 0xBA8,
        FIVE_3 = 0xCA8,
        FIVE_4 = 0xDA8,

        // Команда "6" 0x68 = 0110 1000
        SIX_1 = 0xA68,
        SIX_2 = 0xB68,
        SIX_3 = 0xC68,
        SIX_4 = 0xD68,

        // Команда "7" 0x38 = 0011 1000
        SEVEN_1 = 0xA38,
        SEVEN_2 = 0xB38,
        SEVEN_3 = 0xC38,
        SEVEN_4 = 0xD38,

        // Команда "8" 0x1C = 0001 1100
        EIGHT_1 = 0xA1C,
        EIGHT_2 = 0xB1C,
        EIGHT_3 = 0xC1C,
        EIGHT_4 = 0xD1C,

        // Команда "9" 0x98 = 1001 1000
        NINE_1 = 0xA98,
        NINE_2 = 0xB98,
        NINE_3 = 0xC98,
        NINE_4 = 0xD98,

    } state;


    int index;
    int timerCount;

    int kol;



signals:
    void writeSSS(quint16 data);

    void writeSSS_ISS3(quint8 data);

    void log(QString data);

    void IS_Gui(bool);
    void Free_Gui(bool);


public slots:
    void process_start();

    void readSSS(quint16 data);

    void clearAll();

    void comandCall(QList<quint16> number_1,QList<quint16> number_2);

    void cansel();

    void setNumber_1(QList<quint16> number_1, QList<quint16> number_2);

    void setMod_Onix(quint16 data);

    QString nomer(QList<quint16> number);

    //ISS3

    void ISS3_Receipt_fn(quint8 CNS, quint8 numberSendMSB,quint8 numberSendLSB,quint8 RchVK,quint8 Priorytet);
    void ISS3_Call_fn(quint8 CNS, quint8 numberSendMSB,quint8 numberSendLSB,quint8 numberReceiverMSB,quint8 numberReceiverLSB);

    void ISS3_Free_fn(quint8 CNS, quint8 numberSendMSB,quint8 numberSendLSB,quint8 Rezerv1,quint8 Rezerv2);

    void ISS3_ISS3_Refusal_fn(quint8 CNS, quint8 numberSendMSB,quint8 numberSendLSB,quint8 numberReceiverMSB, quint8 numberReceiverLSB);


private:

    quint16 Mod_Onix_ms;

    quint16 Mod_Onix;
    quint16 Mod_SSS;

    QList<quint16> DefaultNumber_1;
    QList<quint16> DefaultNumber_2;
    QList<quint16> Number_1;
    QList<quint16> Number_2;

    bool setNumber;
    bool flagNumber;


};

#endif // ONIXB_H
