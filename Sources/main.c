#include <hidef.h>   
#include "derivative.h"    
/*
  T0=0UT
  A4-A5=S0-S1
  A6-A7=S2-S3
  GND=OE-GND
  VCC=VCC*/
  
#define DO 45876   //1000/261.63=3.82==>3.82/2==>1.91==>1.91*65535==>125243==>125243/2.73==>45876
#define RE 40872
#define MI 36412
#define FA 34369
#define SOL 30619
#define LA 27278
#define SI 24302
#define DO2 22938

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------Variables-----------------------------------------------------------------------*/
char *r="RED=";
char *g="GREEN=";
char *b="BLUE=";
char *c="C=";
char *space="  ";
int color=0;
int redCandy=0,greenCandy=0,blueCandy=0;
unsigned long int pulse = 0;
unsigned long int redR=0;     
unsigned long int blueB=0;    
unsigned long int greenG=0;  
unsigned long int clearC=0; 
unsigned long int whiteCalRed=0; 
unsigned long int whiteCalGreen=0; 
unsigned long int whiteCalBlue=0; 
unsigned long int blackCalRed=0; 
unsigned long int blackCalGreen=0; 
unsigned long int blackCalBlue=0;  
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 
/*--------------------------------------------------------------------------Functions----------------------------------------------------------------------*/
int max(void);
void upMotor(int x);
void downMotor(int x);
void scaling(void);
void initialize(void);
void redFilter(void);
void blueFilter(void);
void clearFilter(void);
void greenFilter(void);
void red_read(void); 
void blue_read(void); 
void clear_read(void); 
void green_read(void);
void calibration(void); 
void put(char *ptr);
void openLCD(void);
void put2lcd(char c,int type);
void numLCD(unsigned long int color);
void startingSevenSegment(void);
void SNDelay(unsigned int x);
void ringBuzzer(unsigned int note);
long map(long value, long fromLow, long fromHigh, long toLow, long toHigh);
unsigned long int constrain(unsigned long int value, unsigned long int a, unsigned long int b);
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 
void main(void) 
{
  DDRB=0xFF;
  TSCR1=0x80;              //TEN bit enable  
  TSCR2=0x00;              //TOI=0
  TIOS=TIOS & ~0x01;  //D0 bit input capture
  TCTL4= 0x02;             //falling edge
  TIE=0x01;                  //timer interrupt
  TFLG2=0x80;             //timer overflow bit clear
 
  upMotor(23); SNDelay(1);
  initialize();
  scaling();     
  __asm CLI; 
  
  startingSevenSegment();   
  openLCD();      
  calibration();   


  while(1){ 
    upMotor(23); SNDelay(1);
    put2lcd(0x01,0); //clear screen
    SNDelay(366);
    PORTB=0x00;  SNDelay(1);
             
             
    upMotor(17);SNDelay(2);         
    red_read();
    redR=constrain(redR,blackCalRed,whiteCalRed);    
    redR = map(redR, blackCalRed, whiteCalRed,0,255);	SNDelay(5);
  
	green_read();
	greenG=constrain(greenG,blackCalGreen,whiteCalGreen);
	greenG = map(greenG, blackCalGreen,whiteCalGreen, 0,255);SNDelay(5); 
  
	blue_read();
	blueB=constrain(blueB,blackCalBlue,whiteCalBlue);
	blueB = map(blueB,blackCalBlue,whiteCalBlue, 0,255);	SNDelay(5);	 

    clear_read();
	color=max();
		
	
	if(color==1){
	  //color is=red
	  downMotor(9);
	  redCandy++;}	  
	else if(color==2){
	  //color is=green
	  downMotor(13);
	  greenCandy++;}	  
	else if(color==3){
	  //color is=blue
	  downMotor(17);
	  blueCandy++;}		
	else; //color is=unknown

    
	SNDelay(1);upMotor(13);SNDelay(2);
	put(r);numLCD(redCandy);put(space);PORTB=0x03; 
    put(g);numLCD(greenCandy);put2lcd(0xC0,0); PORTB=0x1F; 
    put(b);numLCD(blueCandy);put(space); PORTB=0xFF;
    
    DisableInterrupts;
    ringBuzzer(DO);ringBuzzer(MI);ringBuzzer(RE);ringBuzzer(FA);  //call the buzzer function   one note ring 100ms
    __asm CLI;
    SNDelay(1);SNDelay(2);     
    }            
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------BUZZER------------------------------------------------------------------------------------*/
void ringBuzzer(unsigned int note) {
  unsigned int Tcount;
  unsigned int tof = 0; 

  DDRT=DDRT | 0x20;
  TIOS=TIOS | 0x20;
  TCTL1=TCTL1 | 0x04;
  while( tof <365/8){     
    Tcount = TCNT;
    Tcount = Tcount + note;
    TC5 = Tcount;
    TFLG2 = 0x80;
   
    while(!(TFLG1 & TFLG1_C5F_MASK)); 
    TFLG1 = TFLG1 | TFLG1_C5F_MASK;
      
    if(TFLG2 & 0x80) 
      tof = tof + 1;}
      TCTL1=TCTL1 & ~0x04; 
}        
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------INTERRUPT-------------------------------------------------------------------------------*/ 
interrupt ( ( (0x10000-Vtimch0) /2) -1)  void TC0_ISR(void) {
  pulse++; 
  TFLG1=TFLG1 | 0x01; }    
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/                                                   
/*-----------------------------------------------------------MAP-CONSTRAIN----------------------------------------------------------------------------*/ 
//references:https://www.arduino.cc/en/reference/map
long map(long value, long fromLow, long fromHigh, long toLow, long toHigh){
  return ((value - fromLow) * (toHigh - toLow)) / ((fromHigh - fromLow) + toLow);}
 
//references:https://www.arduino.cc/en/reference/constrain 
unsigned long int constrain(unsigned long int value,unsigned long int a, unsigned long int b){ 
  if(value < a)  return a;
  else if(value > b)  return b;
  else return value;}
 /*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------CALIBRATION-----------------------------------------------------------------------------*/ 
void calibration(){ 
  put("Siyah ve Beyaz "); put2lcd(0xC0,0); put("kagitlari okutun"); 
  SNDelay(1);SNDelay(1); SNDelay(1); 
  put2lcd(0x01,0);

  put("siyah");put2lcd(0xC0,0);put("kalibrasyonu:");      SNDelay(2);
  SNDelay(1);SNDelay(1);SNDelay(1); 
  red_read();    blackCalRed=redR; PORTB=0x03;            SNDelay(4);
  green_read();blackCalGreen=greenG; PORTB=0x1F;    SNDelay(4);
  blue_read();  blackCalBlue=blueB; PORTB=0xFF;          SNDelay(4);
  put2lcd(0x01,0);PORTB=0x00;
  
  
  put("beyaz");put2lcd(0xC0,0);put("kalibrasyonu:");      SNDelay(2); 
  SNDelay(1);SNDelay(1);SNDelay(1);    
  red_read();    whiteCalRed=redR; PORTB=0x03;            SNDelay(4); 
  green_read();whiteCalGreen=greenG; PORTB=0x1F;    SNDelay(4); 
  blue_read();  whiteCalBlue=blueB; PORTB=0xFF;           SNDelay(4);  }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------MOTORS---------------------------------------------------------------------------------*/
//23=left---5=right
void upMotor(int value){  
  PWMPRCLK = 0x03; // Set clock A prescaler to 8
  PWMSCLA = 150; // Set clock A scaler to 150
  PWMCLK = 0x10; // Set clock 4 to use scaled clock A
  PWMPOL = 0x10; // Set clock A for positive polarity pulse
  PWMCAE=0x0; 
  PWMCTL=0x0;	
  PWMPER4 = 200; // Set PWM4 period (200 cycles = 20 ms)
  PWMDTY4 =value; // Set PWM4 duty cycle (6: right,11: middle 17:left)
  PWMCNT4=10;
  PWME = 0x10; // Enable PWM4
}

//23=left---5=right
void downMotor(int value){  
  PWMPRCLK = 0x03; // Set clock A prescaler to 8
  PWMSCLA = 150; // Set clock A scaler to 150
  PWMCLK = 0x20; // Set clock 5 to use scaled clock A
  PWMPOL = 0x20; // Set clock A for positive polarity pulse
  PWMCAE=0x0; 
  PWMCTL=0x0;	
  PWMPER5 = 200; // Set PWM5 period (200 cycles = 20 ms)
  PWMDTY5 =value; // Set PWM5 duty cycle (6: right,11: middle 17:left)
  PWMCNT5=20;
  PWME = 0x20; // Enable PWM5
}   
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------DELAY-----------------------------------------------------------------------------------*/
//SNDelay = 1   ==> 1 sn
//SNDelay = 5   ==> 200 ms
//SNDelay = 10 ==> 100 ms   
void SNDelay(unsigned int x){
  unsigned int overflow=0;
  TFLG2=0x80;
  while(overflow<366/x){    
    while(!(TFLG2 & 0x80));
    overflow++;
    TFLG2 = 0x80;}}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------PHOTODIODE TYPE-----------------------------------------------------------------------*/ 
void initialize(void){ //DDRA=1111-0000
  DDRA=DDRA| 0xF0; 
  DDRT=0xFE; //  DDRT=1111-1110    1=OUTPUT,0=INPUT     Bit 0  input  other pins are output.
  PTT=PTT |0x01;}
    
void scaling(void){
  PORTA=PORTA|0x10;//D4=High  D5=High  %100 f0.
  PORTA=PORTA|0x20;}

void redFilter(void){
  PORTA=PORTA & 0xBF;
  PORTA=PORTA & 0x7F;}
    
void greenFilter(void){
  PORTA = PORTA | 0x40;
  PORTA = PORTA | 0x80;}

void blueFilter(void){
  PORTA = PORTA & 0xBF; 
  PORTA = PORTA | 0x80;}

void clearFilter(void){
  PORTA = PORTA | 0x40; 
  PORTA = PORTA & 0x7F;}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/  
/*---------------------------------------------------------------COLORS----------------------------------------------------------------------------------*/   
int max(void){
  if(blueB>95 && greenG>95 && redR>95){
   // put("WHITE"); 
    return 0;}
  else if(blueB<redR && greenG<redR){
   // put("RED"); 
    return 1;    }
  else if(blueB<=greenG && redR<=greenG) {
  //  put("GREEN");
    return 2;}      
  else if(redR<blueB && greenG<blueB) {
  //  put("BLUE");
    return 3; }
  }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------READ COLOR-----------------------------------------------------------------------------*/   
void red_read(void)  { 
	redFilter(); //select red filter
	redR=0;
    pulse=0;   
    SNDelay(4);
    redR=pulse;}

void green_read(void){
	greenFilter(); //select green filter
	greenG=0;
    pulse=0;   
    SNDelay(4);
	greenG = pulse;  }

void blue_read(void) {
	blueFilter(); //select blue filter
	blueB=0;
    pulse=0;   
    SNDelay(4);
	blueB = pulse;  }

void clear_read(void){
	clearFilter(); //select clear filter
	clearC=0;
    pulse=0;   
    SNDelay(4);
	clearC = pulse;  }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/  
/*---------------------------------------------------------------LCD FUNCTIONS------------------------------------------------------------------------*/ 
void openLCD(void){
  DDRK=0xFF;
  SNDelay(366);
  put2lcd(0x28,0);
  put2lcd(0x0F,0);
  put2lcd(0x06,0);
  put2lcd(0x01,0);//clear screen
  SNDelay(366);}
  
  
void put2lcd(char c,int type){
  char c_lo,c_hi;
  c_hi=(c & 0xF0)>>2;
  c_lo=(c & 0x0F)<<2;
  
  PORTK=PORTK & (~0x01);
  if(type==1)
    PORTK=c_hi | 0x01;
  else
    PORTK=c_hi;
  
  PORTK=PORTK | 0x02;
  __asm(nop); 
  __asm(nop); 
  __asm(nop); 
  
    
  PORTK=PORTK & (~0x02);
  if(type==1)
    PORTK=c_lo | 0x01;
  else
    PORTK=c_lo;
  
  PORTK=PORTK | 0x02;
  __asm(nop); 
  __asm(nop); 
  __asm(nop); 
  PORTK=PORTK & (~0x02);
  SNDelay(366);}


void put(char *ptr) { 
  while(*ptr){
    put2lcd(*ptr,1);
    ptr++;
    SNDelay(366);}}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/    
/*---------------------------------------------------------------WRITE NUMBER LCD -------------------------------------------------------------------*/ 
/*2 digits numbers write the lcd screen*/          
void numLCD(unsigned long int color) {  
    put2lcd(((color/10)%10)+'0',1);
    put2lcd((color%10)+'0',1); }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/    
/*--------------------------------------------------------------SEVEN SEGMENT DISPLAY -------------------------------------------------------------*/    
void startingSevenSegment(void){ 
  DDRP=0xFF;
  PTP=0x0E;
  PORTB=0b01001111;   SNDelay(1); //3
  PORTB=0b01011011;   SNDelay(1); //2
  PORTB=0b00000110;   SNDelay(1); //1
  PTP=0x00;
}    