//////////////////////////////////////////////////////////////////////////////////////////////////////
//Demetrios Doumas 1/19/15  
// Collect Telemetry data
// Determine fight state
// Flight State Operations  
// Seperation after stablization and above 300 meters
// Camera recording descent, maintain camera earth heading
// Fail safe protection 

#include <Servo.h> 
#include <Wire.h>
#include "Adafruit_BMP085.h"
//#include "Timer.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include "Adafruit_TSL2591.h"  

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(2);  // Accelerometer
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(3);  // Magnetometer
Adafruit_BMP085 bmp;                                            // Pressure sensor                                       
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // Light sensor


const float Pi = 3.14159;
// This is the value that gives you minimal rotation on
// a continuous rotation servo.  It is usually about 90.
// adjust this value to give minimal rotation for your servo
const float ServoNeutral = 94;  
 
// This is the desired direction of travel
// expressed as a 0-360 degree compass heading
// 0.0 = North
// 90.0 = East
// 180.0 = South
// 270 = West
const float targetHeading = 0.0;

float Alt=0;
float Old_Alt=0;
float Current_Alt=0;
float Change_Alt=0;

float  lux_reading=0;
float  lux_refrence=0;



float Inside_Temp=0;
float Outside_Temp=0;
float x,y,z, Mx, My, Mz = 0; //init of accelerameter readings
float time=0;
unsigned long time0 = 0;
unsigned long delay1 = 0;
int time_Chrono;
int flight_state = 0;
int trig_Cam=0;
int trig_Sep=0;


Servo myservo;  // create servo object to control a servo
Servo servo;
int pos = 0;    // variable to store the servo position 
 
int hours, minutes, seconds, chdot_time;

// Check whether or not state meet conditions or switch state
int stb_chk=0; //  stablization counter
int sep_chk=0;//   seperation  counter
int descent_chk=0;// desecent counter   

float time1=0;
float time2=0;
float chtime=0;

float pitch=0;
float roll=0;
float old_pitch=0;
float old_roll=0;
float current_pitch=0;
float current_roll=0;
float change_pitch=0;
float change_roll=0;

void setup()
{
  Serial.begin(9600);
    
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

    //BMP Sensor Check
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

  // Magnetometer
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  
  // Light Sensor
  if(!tsl.begin())
  {
     Serial.println("Ooops, no Light sensor detected ... Check your wiring!");
    while(1);
  }
  
  
  
  
   servo.attach(9);  // Attach servo to pin 9
// turn on led if diagnostic check has passed
   pinMode(13, OUTPUT);  /// LED
   pinMode(12,OUTPUT);  // Camera recording
   myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  
  ////////////////////////
   flight_state=1;
  ////////////////////
// Camera Recording -- As soon as the Cansat starts
// Signal camera to turn on, and it immediately start recording  (simulate button press)
//  if (trig_Cam==0){
     digitalWrite(A1, HIGH); 
     delay(3000); 
     digitalWrite(A1, LOW); 
     delay(3000); 
//     trig_Cam=1;
//    }
   
 
Old_Alt= bmp.readAltitude(102156);  


sensors_event_t event; 
accel.getEvent(&event);
x=event.acceleration.x;
y=event.acceleration.y;
z=event.acceleration.z;
  
  pitch= atan(x/sqrt(pow(y,2) + pow(z,2)));
  roll=atan(y/sqrt(pow(x,2) + pow(z,2)));
 
old_pitch= pitch *(180.0/PI);
old_roll= roll * (180.0/PI);  
  



}


void loop()
{
 
// time1=millis(); 
 
Current_Alt= bmp.readAltitude(102156);
Change_Alt= Current_Alt - Old_Alt;
Old_Alt=Current_Alt;
  
//blink three times
//if (i<3){
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  
  //}
  ////////////////////////////////////////////////////////////////////////
  //// Battery Voltage
  int sensorValue = analogRead(A1);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (8.45 / 1023.0);
  // print out the value you read:
  
  /////////////////////////////////////////////////////////////////////////
  ////// Outside Temperature 
   
   int reading = analogRead(A0);  
   float voltage2= reading * 5.0;
   voltage2 /= 1024.0; 
   // now print out the temperature
  Outside_Temp = (voltage2 - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
  
  /////////////////////////////////////////////////////////////////////////
  ///////Inside Temperature 
 
  Inside_Temp = bmp.readTemperature();
  
  ///////////////////////////////////////////////////////////////////////////
  /////////// Altitude
  //////////////////////////////////////////////////////////////////
  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  // 102 561.94
  //101500
  //100 902.8
  
  Alt = bmp.readAltitude(102380); // read in meters
  
  
  //////////////////////////////////////////////////////////////////////////
  //// Axcelerometer
  ///////////////////////////////////////////////////////////////////////////
  sensors_event_t event; 
  accel.getEvent(&event);
  x=event.acceleration.x;
  y=event.acceleration.y;
  z=event.acceleration.z;
  
  pitch= atan(x/sqrt(pow(y,2) + pow(z,2)));
  roll=atan(y/sqrt(pow(x,2) + pow(z,2)));
 
  current_pitch= pitch *(180.0/PI);
  current_roll= roll * (180.0/PI);  
  
  change_pitch=old_pitch-current_pitch; 
  change_roll=old_roll- current_roll;
  
  old_pitch=current_pitch;
  old_roll=current_roll; 
 
 
  
  //////////////////////////////////////////////////////////////////////////
  ///////////////////////Light Sensor
  ///////////////////////////////////////////////////////////////////////
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
   tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  tsl.getEvent(&event);
  lux_reading=event.light;
  
  
  
  ///////////////////////////////////////////////////////////////////////////
  /// Camera Rotation Correction
  //////////////////////////////////////////////////////////////////////
  mag.getEvent(&event);
   // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / Pi;
 
 // Normalize to 0-360
  if (heading < 0)
  {
    heading = 360 + heading;
  }
  
  // Calculate the error between tha measured heading and the target heading.
  float error = heading - targetHeading;
  if (error > 180)
  {
    error = error - 360;  // for angles > 180, correct in the opposite direction.
  }
  // A non-zero difference between the heading and the 
  // targetHeading will bias the servoNeutral value and 
  // cause the servo to rotate back toward the targetHeading.
  // The divisor is to reduce the reaction speed and avoid oscillations
  servo.write(ServoNeutral + error / 4 );
 
  delay(40);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// // Flight States
///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
 
 if(flight_state==1)             // Launch wait
 {
    if(Change_Alt >= 1.5)            // Is the cansat acsending ?
     {flight_state=2;}
 }
 else if(flight_state==2)
 {
   if (lux_reading > lux_refrence)  // Is the cansat outside of the rocket?
       {flight_state=3;}
 }
 else if(flight_state==3)
 {
   
 if((((change_pitch<1)||(change_pitch>-1))&&((change_roll<1)||(change_roll>-1)))/*|| stb_chk==2*/)
    {flight_state=4;}
   // ++stb_chk;

 }
 else if(flight_state==4)
 {
 if((Current_Alt >40) /*|| (sep_chk==2)*/)             //  Is the cansat above 400 meters, if so seperate?
  {flight_state=5;}        
  // ++sep_chk;  
 }
 else if(flight_state==5)
 {
 if((Change_Alt <= -1.5)/*|| descent_chk==2*/)             /// Is cansat is desending?
 {flight_state=6;}
  //++descent_chk;

 }
 else if(flight_state==6)
 {
  if((Change_Alt >-1) && (Change_Alt<1))  // Has the cansat landed
   {flight_state=7;}
   Serial.println("Landed");
 }
  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Flight States Operations  /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 // Release Mechanism
 // -- release when altitude <= 500 m during desecent
 
if((flight_state==5) && (trig_Sep==0)) 
{
          for(pos = 0; pos <= 120; pos += 1) // goes from 0 degrees to 90 degrees 
            {                                  // in steps of 1 degree 
              myservo.write(pos);              // tell servo to go to position in variable 'pos' 
              delay(15);                       // waits 15ms for the servo to reach the position 
            }
            Serial.println("has seperated");
            trig_Sep=1;
}

/// Camera stop recording when Cansat has landed.

if((flight_state==7) &&(trig_Cam==0))
{
   digitalWrite(A1, HIGH); 
     delay(3000); 
     digitalWrite(A1, LOW); 
     delay(3000); 
     trig_Cam=1; 
      Serial.println("Stop Recording");
}








 /////////////////////////////////////////////////////////////////////////////////
 ///////////////Arduino Time 
 //////////////////////////////////////////////////////////////////////////////////
  
// time= (millis()/1000); 
// ++time;
 
 ////////////////////////////////////////////////////////////////////////////////////////////////
 /////////// Chronodot  Time
 /////////////////////////////////////////////////////////////////////////////////////////////
 
 
  // send request to receive data starting at register 0
  Wire.beginTransmission(0x68); // 0x68 is DS3231 device address
  Wire.write((byte)0); // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(0x68, 3); // request three bytes (seconds, minutes, hours)
 
  while(Wire.available())
  { 
     seconds = Wire.read(); // get seconds
     minutes = Wire.read(); // get minutes
     hours = Wire.read();   // get hours
    
    seconds = (((seconds & 0b11110000)>>4)*10 + (seconds & 0b00001111)); // convert BCD to decimal
    minutes = (((minutes & 0b11110000)>>4)*10 + (minutes & 0b00001111)); // convert BCD to decimal
    hours = (((hours & 0b00100000)>>5)*20 + ((hours & 0b00010000)>>4)*10 + (hours & 0b00001111)); // convert BCD to decimal (assume 24 hour mode)
    
    chdot_time=(hours*3600)+(minutes *60)+ seconds;

    
}
 
  //Serial.println("<TEAM ID>,<MISSION_TIME>,<ALT SENSOR>,<Outside_TEMP>,<Inside_TEMP>,<VOLTAGE>,flight state, X,Y,Z");
  Serial.print("4996");
  Serial.print(',');
  Serial.print(chdot_time-14);
  Serial.print(',');
  Serial.print(Alt);//Outside_Temp);
  Serial.print(',');
  Serial.print(Outside_Temp);//Inside_Temp);
  Serial.print(',');
  Serial.print(Inside_Temp);
  Serial.print(',');
  Serial.print(voltage);
  Serial.print(',');
  Serial.print(' ');
  Serial.print(' ');
  Serial.print(' ');
  Serial.print(' ');
  Serial.print(flight_state);
  Serial.print(' ');
  Serial.print(' ');
  Serial.print(' ');
  Serial.print(' ');
  Serial.print(',');
  Serial.print(x);
  Serial.print(',');
  Serial.print(y);
  Serial.print(',');
  Serial.print(z);
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print(lux_reading);
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print(chtime);
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print(Change_Alt);
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('x');
  Serial.print('P');
  Serial.print(change_pitch);
  Serial.print('R');
  Serial.print(change_roll);
  Serial.println();  
  delay(620);
  
time2=millis();
chtime=time1-time2;
}




