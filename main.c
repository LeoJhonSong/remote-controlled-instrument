#include "mbed.h"

#define NOTE_0   00

#define NOTE_B0  31

#define NOTE_C1  33

#define NOTE_CS1 35

#define NOTE_D1  37

#define NOTE_DS1 39

#define NOTE_E1  41

#define NOTE_F1  44

#define NOTE_FS1 46

#define NOTE_G1  49

#define NOTE_GS1 52

#define NOTE_A1  55

#define NOTE_AS1 58

#define NOTE_B1  62

#define NOTE_C2  65

#define NOTE_CS2 69

#define NOTE_D2  73

#define NOTE_DS2 78

#define NOTE_E2  82

#define NOTE_F2  87

#define NOTE_FS2 93

#define NOTE_G2  98

#define NOTE_GS2 104

#define NOTE_A2  110

#define NOTE_AS2 117

#define NOTE_B2  123

#define NOTE_C3  131

#define NOTE_CS3 139

#define NOTE_D3  147

#define NOTE_DS3 156

#define NOTE_E3  165

#define NOTE_F3  175

#define NOTE_FS3 185

#define NOTE_G3  196

#define NOTE_GS3 208

#define NOTE_A3  220

#define NOTE_AS3 233

#define NOTE_B3  247

#define NOTE_C4  262

#define NOTE_CS4 277

#define NOTE_D4  294

#define NOTE_DS4 311

#define NOTE_E4  330

#define NOTE_F4  349

#define NOTE_FS4 370

#define NOTE_G4  392

#define NOTE_GS4 415

#define NOTE_A4  440

#define NOTE_AS4 466

#define NOTE_B4  494

#define NOTE_C5  523

#define NOTE_CS5 554

#define NOTE_D5  587

#define NOTE_DS5 622

#define NOTE_E5  659

#define NOTE_F5  698

#define NOTE_FS5 740

#define NOTE_G5  784

#define NOTE_GS5 831

#define NOTE_A5  880

#define NOTE_AS5 932

#define NOTE_B5  988

#define NOTE_C6  1047

#define NOTE_CS6 1109

#define NOTE_D6  1175

#define NOTE_DS6 1245

#define NOTE_E6  1319

#define NOTE_F6  1397

#define NOTE_FS6 1480

#define NOTE_G6  1568

#define NOTE_GS6 1661

#define NOTE_A6  1760

#define NOTE_AS6 1865

#define NOTE_B6  1976

#define NOTE_C7  2093

#define NOTE_CS7 2217

#define NOTE_D7  2349

#define NOTE_DS7 2489

#define NOTE_E7  2637

#define NOTE_F7  2794

#define NOTE_FS7 2960

#define NOTE_G7  3136

#define NOTE_GS7 3322

#define NOTE_A7  3520

#define NOTE_AS7 3729

#define NOTE_B7  3951

#define NOTE_C8  4186

#define NOTE_CS8 4435

#define NOTE_D8  4699

#define NOTE_DS8 4978

//7个音符组成了美妙的音乐

//低   Do re mi fa so la si do
int melody[] = {  NOTE_C4, NOTE_D4,NOTE_E4, NOTE_F4, NOTE_G4,NOTE_A4,NOTE_B4,NOTE_C5};

//中   Do re mi fa so la si do
int melody2[] = {  NOTE_C5, NOTE_D5,NOTE_E5, NOTE_F5, NOTE_G5,NOTE_A5,NOTE_B5,NOTE_C6};

//高
int melody3[] = {  NOTE_C6, NOTE_D6,NOTE_E6, NOTE_F6, NOTE_G6,NOTE_A6,NOTE_B6,NOTE_C7};

//PWM输出口
PwmOut m(p21);

//Song
float frequency[]={NOTE_C4, NOTE_C4, NOTE_D4,NOTE_E4, NOTE_F4, NOTE_G4,NOTE_A4,NOTE_B4,NOTE_C5, NOTE_D5,NOTE_E5, NOTE_F5, NOTE_G5,
    NOTE_A5,NOTE_B5,NOTE_C6, NOTE_D6,NOTE_E6, NOTE_F6, NOTE_G6,NOTE_A6,NOTE_B6,NOTE_C7};
float beat[]={1.5};
DigitalOut led(p6);
InterruptIn IRIN(p10);    //红外接收口
typedef unsigned int u16;     //对数据类型进行声明定义
typedef unsigned char u8;
u8 IrValue[6];
u8 Time;
int i = 0;

void ReadIr()
{
    u8 j,k;
    u16 err;
    Time=0;
    wait_us(700); //7ms
    if(!IRIN.read())     //确认是否真的接收到正确的信号
    {
        err=1500;               //1000*10us=10ms,超过说明接收到错误的信号
        /*当两个条件都为真是循环，如果有一个条件为假的时候跳出循环，免得程序出错的时
          侯，程序死在这里*/
        while(!IRIN.read())   //等待前面9ms的低电平过去
        {
            wait_us(1);
            err--;
        }
        if(IRIN.read())         //如果正确等到9ms低电平
        {
            err=500;
            while((IRIN.read())&&(err>0))        //等待4.5ms的起始高电平过去
            {
                wait_us(1);
                err--;
            }
            for(k=0;k<4;k++)        //共有4组数据
            {
                for(j=0;j<8;j++)    //接收一组数据
                {
                    err=50;
                    while((!IRIN.read()))//等待信号前面的560us低电平过去
                    {
                        wait_us(1);
                        err--;
                    }
                    err=500;
                    while((IRIN.read())&&(err>0))    //计算高电平的时间长度。
                    {
                        wait_us(100);   //0.1ms
                        Time++;
                        err--;
                        if(Time>30)
                        {
                            return;
                        }
                    }
                    IrValue[k]>>=1;  //k表示第几组数据
                    if(Time>=8)         //如果高电平出现大于565us，那么是1
                    {
                        IrValue[k]|=0x80;
                    }
                    Time=0;     //用完时间要重新赋值
                }
            }
        }
        if(IrValue[2]!=~IrValue[3])
        {

            return;
        }
    }
}

int sound = 0;

int main() {
    while(1)
    {
        IRIN.fall(&ReadIr);
        if(IrValue[2] == 45)
            i = 1;
        else if(IrValue[2] == 25)
            i = 2;
        else if(IrValue[2] == 49)
            i = 3;
        else if(IrValue[2] == 189)
            i = 4;
        else if(IrValue[2] == 17)
            i = 5;
        else if(IrValue[2] == 57)
            i = 6;
        else if(IrValue[2] == 181)
            i = 7;
        else
            i = 0;
        if(i)
        {
            led = 0;
            m.period(1/(2*frequency[i]));
            m=0.5;
            wait(0.25*beat[0]);
            m = 0;
        }
        else
            led = 1;
        i = 0;
        IrValue[2] = 0;

    }
}
