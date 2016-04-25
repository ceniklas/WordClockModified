#include <DS3231.h>
// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);


// Display output pin assignments
#define TO 	Display1=Display1 | (1<<0)  
#define ONE	Display1=Display1 | (1<<1)
#define TWO Display1=Display1 | (1<<2)
#define MINUTES Display1=Display1 | (1<<3)
#define QUARTER	Display1=Display1 | (1<<4)
#define TWENTY	Display1=Display1 | (1<<5)
#define MTEN	Display1=Display1 | (1<<6)
#define UNUSED1	Display1=Display1 | (1<<7)

#define HFIVE	Display2=Display2 | (1<<0)
#define FOUR  Display2=Display2 | (1<<1)
#define EIGHT	Display2=Display2 | (1<<2)
#define SEVEN	Display2=Display2 | (1<<3)
#define THREE	Display2=Display2 | (1<<4)
#define PAST	Display2=Display2 | (1<<5)
#define MFIVE	Display2=Display2 | (1<<6)
#define ITIS Display2=Display2 | (1<<7)

#define ELEVEN	Display3=Display3 | (1<<0)
#define HALF	Display3=Display3 | (1<<1)
#define TWELVE	Display3=Display3 | (1<<2)
#define NINE	Display3=Display3 | (1<<3)
#define HTEN	Display3=Display3 | (1<<4)
#define SIX	Display3=Display3 | (1<<5)
#define OCLOCK Display3=Display3 | (1<<6)
#define UNUSED3 Display3=Display3 | (1<<7)

Time t;
int DST = 0;
int  hour, minute, second;
static unsigned long msTick =0;  // the number of Millisecond Ticks since we last 
                                 // incremented the second counter
char Display1=0, Display2=0, Display3=0;


// hardware constants
int LEDClockPin=6;
int LEDDataPin=7;
int LEDStrobePin=8;

int PWMPin = 9;


void setup()
{
  // initialise the hardware	
  // initialize the appropriate pins as outputs:
  pinMode(LEDClockPin, OUTPUT); 
  pinMode(LEDDataPin, OUTPUT); 
  pinMode(LEDStrobePin, OUTPUT); 

  // Initialize the rtc object
  rtc.begin();
  
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(20, 03, 40);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(25, 04, 2016);   // Set the date to January 1st, 2014
  
  //pinMode(BrightnessPin, INPUT);
  
  
  pinMode(PWMPin, OUTPUT); 
  
  Serial.begin(19200);
  
  msTick=millis();      // Initialise the msTick counter
  //displaytime();        // display the current time
}



void ledsoff(void) { Display1=0; Display2=0; Display3=0; }

void WriteLEDs(void) {
 shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display3);
 shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display2);
 shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display1);
 digitalWrite(LEDStrobePin,HIGH);
 delay(2);
 digitalWrite(LEDStrobePin,LOW); 
}

void selftest(int selftestdelay){
  analogWrite(PWMPin, 255);

  ledsoff(); MTEN; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); HALF; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); QUARTER; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); TWENTY; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); MFIVE; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); MINUTES; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); PAST; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); TO; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); ONE; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); TWO; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); THREE; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); FOUR; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); HFIVE; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); SIX; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); SEVEN; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); EIGHT; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); NINE; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); HTEN; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); ELEVEN; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); TWELVE; WriteLEDs(); delay(selftestdelay); 
  ledsoff(); OCLOCK; WriteLEDs(); delay(selftestdelay); 
}


void displaytime(void){

  // start by clearing the display to a known state
  ledsoff();
  
  //Serial.print("It is ");
  ITIS;
  // now we display the appropriate minute counter
  if ((minute>4) && (minute<10)) { 
    MFIVE; 
    MINUTES; 
    //Serial.print("Five Minutes ");
  } 
  if ((minute>9) && (minute<15)) { 
    MTEN; 
    MINUTES; 
    //Serial.print("Ten Minutes ");
  }
  if ((minute>14) && (minute<20)) {
    QUARTER; 
      //Serial.print("Quarter ");
  }
  if ((minute>19) && (minute<25)) { 
    TWENTY; 
    MINUTES; 
    //Serial.print("Twenty Minutes ");
  }
  if ((minute>24) && (minute<30)) { 
    TWENTY; 
    MFIVE; 
    MINUTES;
    //Serial.print("Twenty Five Minutes ");
  }  
  if ((minute>29) && (minute<35)) {
    HALF;
    //Serial.print("Half ");
  }
  if ((minute>34) && (minute<40)) { 
    TWENTY; 
    MFIVE; 
    MINUTES;
    //Serial.print("Twenty Five Minutes ");
  }  
  if ((minute>39) && (minute<45)) { 
    TWENTY; 
    MINUTES; 
    //Serial.print("Twenty Minutes ");
  }
  if ((minute>44) && (minute<50)) {
    QUARTER; 
    //Serial.print("Quarter ");
  }
  if ((minute>49) && (minute<55)) { 
    MTEN; 
    MINUTES; 
    //Serial.print("Ten Minutes ");
  } 
  if (minute>54) { 
    MFIVE; 
    MINUTES; 
    //Serial.print("Five Minutes ");
  }



  if ((minute <5))
  {
    switch (hour) {
    case 1: 
      ONE; 
      //Serial.print("One ");
      break;
    case 2: 
      TWO; 
      //Serial.print("Two ");
      break;
    case 3: 
      THREE; 
      //Serial.print("Three ");
      break;
    case 4: 
      FOUR; 
      //Serial.print("Four ");
      break;
    case 5: 
      HFIVE; 
      //Serial.print("Five ");
      break;
    case 6: 
      SIX; 
      //Serial.print("Six ");
      break;
    case 7: 
      SEVEN; 
      //Serial.print("Seven ");
      break;
    case 8: 
      EIGHT; 
      //Serial.print("Eight ");
      break;
    case 9: 
      NINE; 
      //Serial.print("Nine ");
      break;
    case 10: 
      HTEN; 
      //Serial.print("Ten ");
      break;
    case 11: 
      ELEVEN; 
      //Serial.print("Eleven ");
      break;
    case 12: 
      TWELVE; 
      //Serial.print("Twelve ");
      break;
    }
  OCLOCK;
  //Serial.println("O'Clock");
  }
  else
    if ((minute < 35) && (minute >4))
    {
      PAST;
      //Serial.print("Past ");
      switch (hour) {
    case 1: 
      ONE; 
      //Serial.println("One ");
      break;
    case 2: 
      TWO; 
      //Serial.println("Two ");
      break;
    case 3: 
      THREE; 
      //Serial.println("Three ");
      break;
    case 4: 
      FOUR; 
      //Serial.println("Four ");
      break;
    case 5: 
      HFIVE; 
      //Serial.println("Five ");
      break;
    case 6: 
      SIX; 
      //Serial.println("Six ");
      break;
    case 7: 
      SEVEN; 
      //Serial.println("Seven ");
      break;
    case 8: 
      EIGHT; 
      //Serial.println("Eight ");
      break;
    case 9: 
      NINE; 
      //Serial.println("Nine ");
      break;
    case 10: 
      HTEN; 
      //Serial.println("Ten ");
      break;
    case 11: 
      ELEVEN; 
      //Serial.println("Eleven ");
      break;
    case 12: 
      TWELVE; 
      //Serial.println("Twelve ");
      break;
      }
    }
    else
    {
      // if we are greater than 34 minutes past the hour then display
      // the next hour, as we will be displaying a 'to' sign
      TO;
      //Serial.print("To ");
      switch (hour) {
      case 1: 
        TWO; 
       //Serial.println("Two ");
       break;
      case 2: 
        THREE; 
      //Serial.println("Three ");
        break;
      case 3: 
        FOUR; 
      //Serial.println("Four ");
        break;
      case 4: 
        HFIVE; 
      //Serial.println("Five ");
        break;
      case 5: 
        SIX; 
      //Serial.println("Six ");
        break;
      case 6: 
        SEVEN; 
      //Serial.println("Seven ");
        break;
      case 7: 
        EIGHT; 
      //Serial.println("Eight ");
        break;
      case 8: 
        NINE; 
      //Serial.println("Nine ");
        break;
      case 9: 
        HTEN; 
      //Serial.println("Ten ");
        break;
      case 10: 
        ELEVEN; 
      //Serial.println("Eleven ");
        break;
      case 11: 
        TWELVE; 
      //Serial.println("Twelve ");
        break;
      case 12: 
        ONE; 
      //Serial.println("One ");
        break;
      }
    }

   WriteLEDs();   
}

void loop(void)
{
  
  //selftest(100);
 
  //Uncomment the following line and comment the next one in order to
  //  enable dimming via a potentiometer connected to pin 0:
  //analogWrite(PWMPin, analogRead(0)/4);
  analogWrite(PWMPin, 255);


  // Send Day-of-Week
  //Serial.print(rtc.getDOWStr());
  // Send date
  //Serial.print(rtc.getDateStr());
  // Send time
  //Serial.println(rtc.getTimeStr());
  
  t = rtc.getTime();
  
  second = t.sec; 
  minute = t.min;
  hour = t.hour + DST;

  hour = (hour > 12) ? hour - 12 : hour;

  if(t.mon < 3){ 
    DST = 0;  
  }
  else if(t.mon == 3 && DST == 0){
    if(t.date >= 25 && t.date <= 31){
      if(t.dow == 7 && t.hour == 2){
        DST = 1;
      }
    }
  }
  else if(t.mon > 3 && t.mon < 10){
    DST = 1;
  }
  else if(t.mon == 10 && DST == 1){
    if(t.date >= 25 && t.date <= 31){
      if(t.dow == 7 && t.hour == 3){
        DST = 0;
      }
    }
  }
  else{
    DST = 0;
  }
  
  /*if (t.dow == 7 && t.mon == 10 && t.date >= 25 && t.date <= 31 && t.hour == 3)
  {
    DST=0;
    //hour = 2;//rtc.setTime(02, 00, 00);     // Set the time to 12:00:00 (24hr format)
  }
  if ((t.dow == 7 && t.mon == 3 && t.date >= 25 && t.date <= 31 && t.hour == 2) || )
  {
    DST=1;
    //rtc.setTime(03, 00, 00);
  }*/

  
  
  Serial.print("The time is: ");
  Serial.print(rtc.getTimeStr());
  Serial.print(" with a temperature of: ");
  Serial.println(rtc.getTemp());
  
  displaytime();

  delay(1000);
}

