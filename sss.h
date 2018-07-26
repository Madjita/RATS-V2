#ifndef SSS_H
#define SSS_H

#include <QObject>

#include <QDebug>
#include <QThread>

#include <QTimer>
#include <QTime>

class Sss : public QObject
{
    Q_OBJECT
public:
    explicit Sss(QObject *parent = nullptr);

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


    int kol;
    int kol2;


signals:
    void writeSSS(quint16 data);
    void writeSSS2(quint16 data);

    void log1(QString data);
    void log2(QString data);

    void activ();

public slots:
     void process_start();

     void readSSS(quint16 data);
     void readSSS2(quint16 data);

     void startSimulation(int);
     void stopSimulation();

     void clearAll();

     void free();

     void setMod_SSS(quint16);
     void setMod_SSS_fz(quint16);
     void setMod_SSS_mod(quint16);

     void setMod_IS();
     void setMod_IS_1();
     void setMod_IS_2();

     QString nomer(QList<quint16> number);


private:
    quint16 Mod_Onix_1;
    quint16 Mod_SSS_1;

    quint16 Mod_Onix_2;
    quint16 Mod_SSS_2;


    quint16 Mod_SSS;
    quint16 Mod_SSS_fz;
    quint16 Mod_SSS_mod;

    bool setNumber;

    bool setNumber_1;


    int count;
    QList<quint16> Number_1;
    QList<quint16> Number_2;


    QTimer* timer;

};

#endif // SSS_H
