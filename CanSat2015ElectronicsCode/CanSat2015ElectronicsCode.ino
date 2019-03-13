//////////////////////////////////////////////////////////////////////////////////////////////////////
//Demetrios Doumas 1/19/15
/*CanSat Competition 2015 Objectives*/  
// Collect Telemetry data such as accelerameter data, voltage, temperature, and air pressure.
// Determine fight state of this can-sized satellite.
// Needs to perform Flight State Operations such as:
// Seperation after stablization and an altitude over 300 meters
// Camera recording flight_path.

#include <Servo.h> 
#include <Wire.h>
#include "Adafruit_BMP085.h"
#include "Timer.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include "Adafruit_TSL2591.h"

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(2);  // Accelerometer
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(3);  // Magnetometer
Adafruit_BMP085 bmp;                                            // Pressure sensor                                       
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // Light sensor


const float Pi = 3.14159;

float Alt=0;
float Old_Alt=0;
float Current_Alt=0;
float Change_Alt=0;

float  lux_reading=0;
float  lux_refrence=0;

float Inside_Temp=0;
float Outside_Temp=0;
float x=0;
float y=0;
float z=0;
float chdot_time=0;
int flight_state = 0;
int trig_Cam=0;
int trig_Sep=0;


Servo release_servo;  // release mechanism servo
int pos = 0;    // variable to store the servo position 
 
int hours, minutes, seconds, chdot_time;

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
    
  /* Checking sensor connections */
  if(!accel.begin())  // Checking accelerometer connection
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

    
  if (!bmp.begin()) // Checking pressure sensor connection
  {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

  if(!mag.begin()) // Checking magnetometer connection
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  if(!tsl.begin()) // Checking Light sensor connections
  {
     Serial.println("Ooops, no Light sensor detected ... Check your wiring!");
    while(1);
  }
  
 
   pinMode(13, OUTPUT);  /// LED
   pinMode(12,OUTPUT);  // Camera 
   release_servo.attach(9);  // attaches the servo on pin 9 to the servo object
   
    Old_Alt= bmp.readAltitude(102156);  // The parameter changes dependig on the weather and location and needs to be adjusted
       
    sensors_event_t event; 
    accel.getEvent(&event);
    x=event.acceleration.x;
    y=event.acceleration.y;
    z=event.acceleration.z;
      
    pitch= atan(x/sqrt(pow(y,2) + pow(z,2)));
    roll=atan(y/sqrt(pow(x,2) + pow(z,2)));
     
    old_pitch= pitch *(180.0/PI);
    old_roll= roll * (180.0/PI);  

    ////////////////////////
    /// Cansat is waiting to launch
    flight_state=1;
    ////////////////////
  
    // Camera Recording -- As soon as the Cansat starts
    // Signal camera to turn on, and it immediately start recording  (simulate button press)

    digitalWrite(12, HIGH); 
    delay(3000); 
    digitalWrite(12, LOW); 
    delay(3000); 
}


void loop()
{
 
Current_Alt= bmp.readAltitude(102156);
Change_Alt= Current_Alt - Old_Alt;
Old_Alt=Current_Alt;
  
  // The led Light is blinking, indicating that the cansat is sending telemetry data and has passed through the setup.
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  

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
   if((((change_pitch<1)||(change_pitch>-1))&&((change_roll<1)||(change_roll>-1))))    // Has the cansat reached its zenith point?
    {flight_state=4;}
 }
 else if(flight_state==4)
 {
   if((Current_Alt >300))             //  Is the cansat above 300 meters, if so seperate?
    {flight_state=5;}        
  
 }
 else if(flight_state==5)
 {
   if((Change_Alt <= -1.5))             /// Is cansat is desending?
   {flight_state=6;}

 }
 else if(flight_state==6)
 {
   if((Change_Alt >-1) && (Change_Alt<1))  // Has the cansat landed ?
    {flight_state=7;}
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
            release_servo.write(pos);         // tell servo to go to position in variable 'pos' 
            delay(15);                       // waits 15ms for the servo to reach the position 
          }
            trig_Sep=1;
}

/// Camera stop recording when Cansat has landed.

if((flight_state==7) &&(trig_Cam==0))
{
     digitalWrite(12, HIGH); 
     delay(3000); 
     digitalWrite(12, LOW); 
     delay(3000); 
     trig_Cam=1; 
}



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
  Serial.print(chdot_time-14); // The 14 is the offset
  Serial.print(',');
  Serial.print(Alt);//Outside_Temp);
  Serial.print(',');
  Serial.print(Outside_Temp);//Inside_Temp);
  Serial.print(',');
  Serial.print(Inside_Temp);
  Serial.print(',');
  Serial.print(voltage);
  Serial.print(',');
  Serial.print(flight_state);
  Serial.print(',');
  Serial.print(x);
  Serial.print(',');
  Serial.print(y);
  Serial.print(',');
  Serial.print(z);
  Serial.println();  
  delay(620);
  

}




