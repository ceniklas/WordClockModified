//Transistor BC327 med 1k Ohm

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68

// Display output pin assignments
#define TO   Display1=Display1 | (1<<0)  
#define ONE Display1=Display1 | (1<<1)
#define TWO Display1=Display1 | (1<<2)
#define MINUTES Display1=Display1 | (1<<3)
#define QUARTER Display1=Display1 | (1<<4)
#define TWENTY  Display1=Display1 | (1<<5)
#define MTEN  Display1=Display1 | (1<<6)
#define UNUSED1 Display1=Display1 | (1<<7)

#define HFIVE Display2=Display2 | (1<<0)
#define FOUR  Display2=Display2 | (1<<1)
#define EIGHT Display2=Display2 | (1<<2)
#define SEVEN Display2=Display2 | (1<<3)
#define THREE Display2=Display2 | (1<<4)
#define PAST  Display2=Display2 | (1<<5)
#define MFIVE Display2=Display2 | (1<<6)
#define ITIS Display2=Display2 | (1<<7)

#define ELEVEN  Display3=Display3 | (1<<0)
#define HALF  Display3=Display3 | (1<<1)
#define TWELVE  Display3=Display3 | (1<<2)
#define NINE  Display3=Display3 | (1<<3)
#define HTEN  Display3=Display3 | (1<<4)
#define SIX Display3=Display3 | (1<<5)
#define OCLOCK Display3=Display3 | (1<<6)
#define UNUSED3 Display3=Display3 | (1<<7)

char Display1=0, Display2=0, Display3=0;

int MinuteButtonPin=2;
int HourButtonPin=3;

byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

// hardware constants
int LEDClockPin=6;
int LEDDataPin=7;
int LEDStrobePin=8;
int PWMPin = 9;

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void ledsoff(void) 
{
  Display1=0;
  Display2=0;
  Display3=0;
}

void WriteLEDs(void) 
{
  // Now we write the actual values to the hardware
  shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display3);
  shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display2);
  shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display1);
  digitalWrite(LEDStrobePin,HIGH);
  //delay(2);
  digitalWrite(LEDStrobePin,LOW); 
}

void setup()
{
  Wire.begin();
  Serial.begin(19200);

  // initialise the hardware  
  // initialize the appropriate pins as outputs:
  pinMode(LEDClockPin, OUTPUT); 
  pinMode(LEDDataPin, OUTPUT); 
  pinMode(LEDStrobePin, OUTPUT); 
  //pinMode(BrightnessPin, INPUT);
  pinMode(MinuteButtonPin, INPUT); 
  pinMode(HourButtonPin, INPUT);
  digitalWrite(MinuteButtonPin, HIGH);  //set internal pullup
  digitalWrite(HourButtonPin, HIGH); //set internal pullup

  pinMode(PWMPin, OUTPUT); 
  
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(45,55,0,6,9,10,15);
}

void incrementTime(byte addHour, byte addMinute)
{
  if(minute >= 59 && addMinute == 1)
  {
    setDS3231time(second, 0, 
    hour + 1, dayOfWeek, dayOfMonth, 
    month, year);
  }
  else if(hour >= 23 && addHour == 1)
  {
    setDS3231time(second, minute, 
    1, dayOfWeek, dayOfMonth, 
    month, year);
  }
  else
  {
    setDS3231time(second, minute + addMinute, 
    hour + addHour, dayOfWeek, dayOfMonth, 
    month, year);
  }
}

void setDS3231time(byte tsecond, byte tminute, byte thour, 
byte tdayOfWeek, byte tdayOfMonth, byte tmonth, byte tyear)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(tsecond)); // set seconds
  Wire.write(decToBcd(tminute)); // set minutes
  Wire.write(decToBcd(thour)); // set hours
  Wire.write(decToBcd(tdayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(tdayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(tmonth)); // set month
  Wire.write(decToBcd(tyear)); // set year (0 to 99)
  Wire.endTransmission();
}

void readDS3231time()//byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  second = bcdToDec(Wire.read() & 0x7f);
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0x3f);
  dayOfWeek = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
}

void calculateWordClockTime(byte hour, byte minute)
{
  /*Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(" ");*/
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
    switch (hour) 
    {
      case 1: 
      case 13: 
        ONE; 
        //Serial.print("One ");
        break;
      case 2:
      case 14:  
        TWO; 
        //Serial.print("Two ");
        break;
      case 3: 
      case 15:  
        THREE; 
        //Serial.print("Three ");
        break;
      case 4:
      case 16:   
        FOUR; 
        //Serial.print("Four ");
        break;
      case 5:
      case 17:
        HFIVE; 
        //Serial.print("Five ");
        break;
      case 6:
      case 18:
        SIX; 
        //Serial.print("Six ");
        break;
      case 7:
      case 19: 
        SEVEN; 
        //Serial.print("Seven ");
        break;
      case 8: 
      case 20:
        EIGHT; 
        //Serial.print("Eight ");
        break;
      case 9:
      case 21:
        NINE; 
        //Serial.print("Nine ");
        break;
      case 10:
      case 22:
        HTEN; 
        //Serial.print("Ten ");
        break;
      case 11:
      case 23:
        ELEVEN; 
        //Serial.print("Eleven ");
        break;
      case 12:
      case 24:
        TWELVE; 
        //Serial.print("Twelve ");
        break;
    }
  OCLOCK;
  //Serial.println("O'Clock");
  }
  else
    if ((minute < 35) && (minute > 4))
    {
      PAST;
      //Serial.print("Past ");
      switch (hour) 
      {
      case 1:
      case 13:
        ONE; 
        //Serial.println("One ");
        break;
      case 2:
      case 14:
        TWO; 
        //Serial.println("Two ");
        break;
      case 3:
      case 15:
        THREE; 
        //Serial.println("Three ");
        break;
      case 4:
      case 16:
        FOUR; 
        //Serial.println("Four ");
        break;
      case 5:
      case 17:
        HFIVE; 
        //Serial.println("Five ");
        break;
      case 6:
      case 18:
        SIX; 
        //Serial.println("Six ");
        break;
      case 7:
      case 19:
        SEVEN; 
        //Serial.println("Seven ");
        break;
      case 8:
      case 20:
        EIGHT; 
        //Serial.println("Eight ");
        break;
      case 9:
      case 21:
        NINE; 
        //Serial.println("Nine ");
        break;
      case 10:
      case 22:
        HTEN; 
        //Serial.println("Ten ");
        break;
      case 11:
      case 23:
        ELEVEN; 
        //Serial.println("Eleven ");
        break;
      case 12:
      case 24:
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
      case 13:
        TWO; 
        //Serial.println("Two ");
        break;
      case 2:
      case 14:
        THREE; 
        //Serial.println("Three ");
        break;
      case 3:
      case 15:
        FOUR; 
        //Serial.println("Four ");
        break;
      case 4:
      case 16:
        HFIVE; 
        //Serial.println("Five ");
        break;
      case 5:
      case 17:
        SIX; 
        //Serial.println("Six ");
        break;
      case 6:
      case 18:
        SEVEN; 
        //Serial.println("Seven ");
        break;
      case 7:
      case 19:
        EIGHT; 
        //Serial.println("Eight ");
        break;
      case 8:
      case 20:
        NINE; 
        //Serial.println("Nine ");
        break;
      case 9:
      case 21:
        HTEN; 
        //Serial.println("Ten ");
        break;
      case 10:
      case 22:
        ELEVEN; 
        //Serial.println("Eleven ");
        break;
      case 11:
      case 23:
        TWELVE; 
        //Serial.println("Twelve ");
        break;
      case 12:
      case 24:
        ONE; 
        //Serial.println("One ");
        break;
      }
    }
}

void displayTime()
{
  //byte second, dayOfWeek, dayOfMonth, month, year;
  
  // retrieve data from DS3231
  readDS3231time();//&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  //Turn off
  ledsoff();
  //Calculate the wordclock-time
  calculateWordClockTime(hour, minute);
  Serial.println();
  WriteLEDs();
  
  // send it to the serial monitor
  Serial.print(hour, DEC);
  
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}

void selftest(void){
  analogWrite(PWMPin, 255);

  ledsoff(); MTEN; WriteLEDs(); delay(50); 
  ledsoff(); HALF; WriteLEDs(); delay(50); 
  ledsoff(); QUARTER; WriteLEDs(); delay(50); 
  ledsoff(); TWENTY; WriteLEDs(); delay(50); 
  ledsoff(); MFIVE; WriteLEDs(); delay(50); 
  ledsoff(); MINUTES; WriteLEDs(); delay(50); 
  ledsoff(); PAST; WriteLEDs(); delay(50); 
  ledsoff(); TO; WriteLEDs(); delay(50); 
  ledsoff(); ONE; WriteLEDs(); delay(50); 
  ledsoff(); TWO; WriteLEDs(); delay(50); 
  ledsoff(); THREE; WriteLEDs(); delay(50); 
  ledsoff(); FOUR; WriteLEDs(); delay(50); 
  ledsoff(); HFIVE; WriteLEDs(); delay(50); 
  ledsoff(); SIX; WriteLEDs(); delay(50); 
  ledsoff(); SEVEN; WriteLEDs(); delay(50); 
  ledsoff(); EIGHT; WriteLEDs(); delay(50); 
  ledsoff(); NINE; WriteLEDs(); delay(50); 
  ledsoff(); HTEN; WriteLEDs(); delay(50); 
  ledsoff(); ELEVEN; WriteLEDs(); delay(50); 
  ledsoff(); TWELVE; WriteLEDs(); delay(50); 
  ledsoff(); OCLOCK; WriteLEDs(); delay(50); 
}

void loop()
{
  //selftest();
  analogWrite(PWMPin, 255);
  //ledsoff(); SEVEN; WriteLEDs(); delay(50); 
  displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(1000); // every second

  if ((digitalRead(HourButtonPin)==0 )) 
  {
    Serial.println("Pressed hour");
    incrementTime(1,0);
  }

  if ( (digitalRead(MinuteButtonPin) ==0 ))
  {
    Serial.println("Pressed minute");
    incrementTime(0,1);
  }
     

  
}
