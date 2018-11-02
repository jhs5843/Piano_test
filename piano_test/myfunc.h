#ifndef MYFUNC_H_
#define MYFUNC_H_
 
#include <avr/io.h>
#define F_CPU 16000000
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"
#include <stdlib.h>
#include <avr/interrupt.h>
 
#define POWER_ON_MODE 0
#define POWER_OFF_MODE 1
 
#define NOMAL_MODE 2
#define MENU_SELECT_MODE 3
 
#define RECORD_MODE 4
#define LISTEN_MODE 5
#define EXAM_SELECT_MODE 6
 
#define EXAM1_MODE 7
#define EXAM2_MODE 8
#define EXAM3_MODE 9
 
#define POWER_ON_DISPLAY 0
#define POWER_ON_UNDISPLAY 1
 
#define POWER_OFF_DISPLAY 0
#define POWER_OFF_UNDISPLAY 1
 
//메뉴
#define MENU_RECORD_MODE 0
#define MENU_LISTEN_MODE 1
#define MENU_EXAM_MODE 2
#define MENU_RETURN_MODE 3
 
//EXAM
#define EXAM1 0
#define EXAM2 1
#define EXAM3 2
#define EXAMRETURN 3
 
//음계
#define DO 261.63
#define RE 293.66
#define MI 329.63
#define FA 349.23
#define SOL 391.99
#define RA 440.0
#define SI 493.88
#define NO 0
 
#define DOUP 277.18
#define REUP 311.12
#define FAUP 369.99
#define SOLUP 415.30
#define RAUP 466.16
 
//KEY값
#define K_MENU  0b00101011
#define K_POWER 0b01001011
#define K_DO 0b00011110
#define K_RE 0b00101110
#define K_MI 0b1001110
#define K_FA 0b00011101
#define K_SOL 0b00101101
#define K_RA 0b01001101
#define K_SI 0b00011011
 
#define LED_DO 0b11111110
#define LED_RE 0b11111101
#define LED_MI 0b11111011
#define LED_FA 0b11110111
#define LED_SOL 0b11101111
#define LED_RA 0b11011111
#define LED_SI 0b10111111
 
//전역 변수
unsigned int count =0;
unsigned int mode =0,state =0;
unsigned int menuSelectCount =0;
unsigned int ExamSelectCount =0;
unsigned int timeCount =0,sec =0;
unsigned int PowerOnDisplayState =0,PowerOffDisplayState=0;
unsigned int key=0;
 
unsigned int tCount1 = 0, tCount2 = 0;
unsigned int msec = 0, rotSec = 0;
unsigned int note[100] = {0,};
unsigned int note_time[100] = {0,};
unsigned int n_index = 0, n_flag = 0;
 
void port_init(){
    TCCR1A =0x40;
    TCCR1B = 0x09;
    
    TIMSK = 0xc0; // timer2 enable
    TCCR2 = 0x03; // 64분주
    TCNT2 = 256-(256-6); // 0.001s = 1ms
}
 
void powerOn()
{
    if (PowerOnDisplayState == POWER_ON_DISPLAY){
        lcd_putsf(0,0,(unsigned char *)"  PIANO START!  ");
        lcd_putsf(0,1,(unsigned char *)"    POWER ON!   ");
        _delay_ms(3000);
        PowerOnDisplayState =1;
    }
    else if(PowerOnDisplayState==POWER_ON_UNDISPLAY){
        lcd_putsf(0,0,(unsigned char *)"  PIANO START!  ");
        lcd_putsf(0,1,(unsigned char *)"                ");
        mode = NOMAL_MODE;
    }
}
 
void powerOff()
{
    if(PowerOffDisplayState == POWER_OFF_DISPLAY){
        lcd_putsf(0,0,(unsigned char *)"  PIANO SYSTEM  ");
        lcd_putsf(0,1,(unsigned char *)"   POWER OFF!   ");
        _delay_ms(1000);
        lcd_putsf(0,0,(unsigned char *)"    GOOD BYE    ");
        lcd_putsf(0,1,(unsigned char *)"   POWER OFF!   ");
        _delay_ms(1000);
        PowerOffDisplayState =POWER_OFF_UNDISPLAY;
    }
    else if(PowerOffDisplayState == POWER_OFF_UNDISPLAY){
        lcd_putsf(0,0,(unsigned char *)"                ");
        lcd_putsf(0,1,(unsigned char *)"                ");
    }
}
 
 
// Key Matrix 함수
unsigned char getKey(){
    unsigned char key = 0;
    for (int i=0; i<3;i++){
        if (i==0)PORTD = 0b11111110;
        if (i==1)PORTD = 0b11111101;
        if (i==2)PORTD = 0b11111011;
        
        _delay_ms(5);
        
        key = (~PIND & 0xf0);
        if (key){
            return key|(PORTD & 0x0f);
        }
    }
    return 0;
}
 
// 음계 출력 Buzzer(주파수, 출력횟수)
void Buzzer(double hz,int count){
    for (int i=0;i<count; i++){
        PORTB = 0xff;
        _delay_ms(((double)1000/hz)/2);
        PORTB = 0x00;
        _delay_ms(((double)1000/hz)/2);
    }
}
 
//CTC 모드로 소리 출력
void freqSet(int freq, int delayTime){
    if (freq == 0) OCR1A = 0;
    else OCR1A = 8000000 /freq;
    _delay_ms(delayTime);
}
 
void freq(int freq){
    if (freq == 0) OCR1A = 0;
    else OCR1A = 8000000 /freq;
}
 
// 음악 저장
int doremi[10] = {DO,RE,MI,FA,SOL,RA,SI,(DO*2)};
int school[100]= {SOL,SOL,RA,RA,SOL,SOL,MI, SOL,SOL,MI,MI,RE,SOL,SOL,RA,RA,SOL,SOL,MI,SOL,MI,RE,MI,DO};
int plane [100]= {MI,RE,DO,RE,MI,MI,MI,RE,RE,RE,MI,MI,MI,MI,RE,DO,RE,MI,MI,MI,RE,RE,MI,RE,DO};
 
 
int dog[100]    = {RA,RA,RA,SOL,RA,SOL,FA,2*FA,2*FA,2*MI,RA,2*RE,
                2*RE,2*RE,2*DO,RAUP,RA,SOL,FA,FA,SOL,SOL,2*DO,
                RA,RA,RA,SOL,RA,SOL,FA,2*FA,2*FA,2*MI,RA,2*RE,
                2*RE,2*RE,2*DO,RAUP,RA,SOL,FA,RE,FA,MI,FA,SOL,FA,
                RA,RAUP,RA,2*RE,2*MI,2*FA,2*MI,RA,2*RE,2*RE,2*DO,RAUP,RA,
                SOL,RA,RAUP,2*DO,2*FA,2*MI,2*RE,2*FA,2*FA,2*FA,2*MI,2*RE,2*DO,2*DO,
                2*FA,2*MI,2*FA,2*MI,2*RE,2*RE,2*RE,2*RE,2*DO,2*RE,2*DO,RA,
                FA,FA,SOL,SOL,2*DO,RA,FA,RE,FA,MI,FA,SOL,FA};
int dogtime[100]= {250,500,250,375,125,500,2000,250,750,500,500,2000,
                250,750,500,500,750,250,1000,1000,250,750,2000,
                250,500,250,375,125,500,2000,250,750,500,500,2000,
                250,750,500,500,750,250,1000,250,750,375,500,2000,
                375,125,375,1125,375,125,375,1125,250,750,500,500,2000,
                250,500,250,1000,500,500,1000,250,500,250,500,500,1500,500,
                1000,375,125,375,2125,250,500,250,375,125,500,2000,
                250,375,500,500,500,500,1000,250,750,375,125,500,2000};
int snow[500] ={RE,SI,RA,SI,2*RE,RA,NO,RE,RE,MI,SOL,SOL,MI,RE,NO,SOL,RA,
                SI,RA,SI,2*MI,2*RE,SI,NO,SI,SI,2*DO,SI,RA,SOL,SI,RA,NO,RE,SI,RA,SI,2*RE,RA,NO,RE,RE,
                MI,SOL,SOL,MI,RE,NO,SOL,RA,SI,RA,SI,2*MI,2*RE,SI,NO,SI,SI,2*DO,SI,RA,SOL,SOL,NO,
                NO,MI,SOL,2*RE,2*DO,SI,RA,RA,SOL,FAUP,SOL,MI,SOL,
                2*RE,2*MI,2*RE,2*DO,SI,RA,SI,NO,SOL,SOL,2*MI,2*FAUP,2*SOL,2*FAUP,2*MI,2*DOUP,2*RE,
                NO,2*MI,(2*FA),2*MI,2*RE,2*DO,SI,SOL,SOL,NO,MI,SOL,2*MI,2*REUP,2*MI,2*FAUP,SI,SI,
                2*SOL,2*FAUP,2*SOL,2*FAUP,2*SOL,2*FAUP,2*MI,2*RE,2*MI,2*SI,2*SI,NO,2*FAUP,2*SOL,2*RA,2*FAUP,2*MI,2*RE,2*RE,NO,SI,2*DO,2*RE,
                2*RE,2*DO,2*DO,2*MI,2*RE,SI,RA,SOL,NO,MI,SOL,2*RE,2*DO,2*DO,2*MI,2*MI,2*REUP,2*MI,2*FAUP,2*SOL,2*FAUP,2*SOL,2*FAUP,2*SOL,2*FAUP,2*MI,2*RE,
                2*MI,2*SI,2*SI,NO,2*FAUP,2*SOL,2*RA,2*FAUP,2*MI,2*MI,2*RE,2*RE,2*RA,2*SOL,SI,2*DO,2*RE,2*RE,2*DO,2*DO,2*RE,SI,RA,SOL,SOL,
                NO,SI,2*RE,2*RE,2*DO,DO,RE,2*MI,2*SOL,2*FAUP,FAUP,2*MI,2*FAUP,2*SOL,3*SOL,NO,RE};
int snowtime[500]={250,250,250,250,125,625,250,125,125,250,125,250,125,750,250,125,125,
                250,250,250,125,185,185,250,125,125,250,125,250,125,125,625,250,250,250,250,250,125,625,250,125,125,
                250,125,250,250,750,250,125,125,250,250,250,125,250,375,250,125,125,250,125,250,125,750,500,
                500,250,250,1250,250,250,250,250,250,250,750,250,250,
                750,125,375,125,250,125,1250,500,250,250,750,250,250,250,250,125,125,
                500,250,250,250,250,250,250,250,125,625,500,250,250,625,125,125,625,250,250,
                250,250,250,125,375,250,250,250,250,125,625,125,250,250,250,125,125,250,125,625,500,250,125,125,
                250,250,250,125,250,125,125,125,125,125,125,250,250,250,250,125,375,250,250,250,250,250,250,125,375,250,250,250,
                };
void snowplay(){
    for(int i =0; i<500;i++){
        freqSet(snow[i],snowtime[i]);
        freqSet(0,10);
    }mode = EXAM_SELECT_MODE;
}
void dogPlay(){
    for (int i= 0;i<100;i++){
        freqSet(dog[i],dogtime[i]);
        freqSet(0,10);
    }
    mode = EXAM_SELECT_MODE;
}
void schoolPlay(){
    for (int i= 0;i<25;i++){
        freqSet(school[i],500);
        freqSet(0,200);
    }
    mode = EXAM_SELECT_MODE;
}
void doremiPlay(){
    for (int i= 0;i<8;i++){
        freqSet(doremi[i],500);
        freqSet(0,200);
    }
    mode = EXAM_SELECT_MODE;
}
void key_init(){
    lcd_putsf(0,0,(unsigned char *)"  PIANO START!  ");
    lcd_putsf(0,1,(unsigned char *)"                ");
    char keyPos = 25;
    char strConv[16];
    
    if (key == K_DO){ PORTA = LED_DO; freq(DO); keyPos =1;}
    else if (key==K_RE){ PORTA = LED_RE; freq(RE); keyPos=3;}
    else if (key == K_MI){ PORTA = LED_MI; freq(MI); keyPos =5;}
    else if (key == K_FA){ PORTA = LED_FA; freq(FA); keyPos =7;}
    else if (key == K_SOL){ PORTA = LED_SOL; freq(SOL); keyPos=9;}
    else if (key == K_RA){ PORTA = LED_RA; freq(RA); keyPos=11;}
    else if(key ==K_SI){ PORTA = LED_SI; freq(SI); keyPos =13;}
    else
    {
        OCR1A=0;
        PORTA = 0xff;
        sprintf(strConv,"                ");
        lcd_putsf(0,1,(unsigned char *)strConv);
    }
    
    sprintf(strConv,"                ");
    strConv[keyPos] = 0xff;
    lcd_putsf(0,1,(unsigned char *)strConv);
    
}
void recordDisplay(){
    lcd_putsf(0,0,(unsigned char *)"  Recording..   ");
    lcd_putsf(0,1,(unsigned char *)"                ");
}
void ListenDisplay(){
    lcd_putsf(0,0,(unsigned char *)"  Listening..   ");
    lcd_putsf(0,1,(unsigned char *)"                ");
}
 
 
void keyPowerOff(){
    if (key == K_POWER){ // POWER KEY 입력시 OFF 모드로 이동한다.
        mode = POWER_OFF_MODE;
        PowerOffDisplayState = POWER_OFF_DISPLAY;
        key =0;
        _delay_ms(100);
    }
}
void keyPowerOn(){
    if (key == K_POWER){
        mode = POWER_ON_MODE;
        PowerOnDisplayState = POWER_ON_DISPLAY;
        key =0;
        _delay_ms(100);
    }
}
 
 
void listen(){
    for (int i=1; i<n_index; i++)
    {
        freq(note[i]);
        _delay_ms(note_time[i] * 25);
    }
    freq(0);
    mode = MENU_SELECT_MODE; 
}
#endif /* MYFUNC_H_ */