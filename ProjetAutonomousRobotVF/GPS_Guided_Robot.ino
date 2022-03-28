#include <AFMotor.h>                                              // the Adafruit Motor Shield Library 
#include "Wire.h"                                                 // Used by I2C and HMC5883L compass
#include "I2Cdev.h"                                               // I2C Communications Library (used for compass)
#include <Servo.h>                                                // Servo library to control Servo arm for metal detector
#include <SoftwareSerial.h>                                       // Software Serial for Serial Communications - not used
#include "TinyGPS.h"                                              // Tiny GPS Plus Library                                                                 
#include <Wire.h>                                                 // for the Compass
#include "DFRobot_QMC5883.h"                                      // for the Compass
#include <LiquidCrystal.h>                                        // for the LCD screen 
//******************************************************************************************************                                                                  
// GPS Variables & Setup

int GPS_Course;                                                   // variable to hold the gps's determined course to destination
int nb_sats;                                                      // variable to hold the number of satellites acquired
TinyGPS gps;
SoftwareSerial ss(12, 13);

//******************************************************************************************************
// Compass Variables & Setup

DFRobot_QMC5883 compass;                                           // HMC5883L compass- we use a QMC actually but it's working close to the HMC

int16_t mx, my, mz;                                                // variables to store x,y,z axis from compass (HMC5883L)
int desired_heading;                                               // initialize variable - stores value for the new desired heading
int compass_heading;                                               // initialize variable - stores value calculated from compass readings
int compass_dev = 5;                                               // the amount of deviation that is allowed in the compass heading - Adjust as Needed

int Heading_A;                                                     // variable to store compass heading
int Heading_B;                                                     // variable to store compass heading in Opposite direction
int pass = 0;                                                      // variable to store which pass the robot is on

//******************************************************************************************************
// Servo Control

Servo myservo;                                                     // create servo object to control a servo 
int pos = 0;                                                       // variable to store the servo position

//*****************************************************************************************************
// GPS Locations

unsigned long Distance_To_Home;                                    // variable for storing the distance to destination

int ac =0;                                                         // GPS array counter
int wpCount = 0;                                                   // GPS waypoint counter
double Home_LATarray[50];                                          // variable for storing the destination Latitude - Only Programmed for 5 waypoint
double Home_LONarray[50];                                          // variable for storing the destination Longitude - up to 50 waypoints

int increment = 0;


//********************************************************************************************************
// LCD Screen
//LiquidCrystal lcd(9,8,5,4,3,2);

//********************************************************************************************************
// Setup

void setup() 
{  
  // LCD SETUP
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Moov'Bot");
  lcd.setCursor(3 ,1);
  lcd.print("Equipe 103");
  
  // Motor Pins
  pinMode(PWM1,OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(PWM2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);

  //Serial baud rates
  Serial.begin(115200);                                            // Serial 0 is for communication with the serial monitor
  hc06.begin(4800);                                                // Serial hc06 is for Bluetooth communication - DO NOT MODIFY - (Before 9600 / Now 4800) - ALREADY IN BLUETOOTH() FCT
  //ss.begin(9600);                                                  // Serial ss is for GPS communication - DO NOT MODIFY - (Now 9600) - ALREADY IN GETGPS() FCT
  //myservo.attach(9);                                               // attaches the servo to pin 9 (Servo 0 on the Adafruit Motor Control Board)
  Serial.println("Serials OK");

  // Ping Sensor
    pinMode(TRIGGER_PIN, OUTPUT);
    digitalWrite(TRIGGER_PIN, LOW); 
    pinMode(ECHO_PIN, INPUT);
    Serial.println("Ultrasonic Sensor OK");
  
  // Compass
  while (!compass.begin())
  {
    Serial.println("Could not find a valid Compass sensor, check wiring!");
    delay(500);
  }

    if(compass.isHMC()){
        compass.setRange(HMC5883L_RANGE_1_3GA);
        compass.setMeasurementMode(HMC5883L_CONTINOUS);
        compass.setDataRate(HMC5883L_DATARATE_15HZ);
        compass.setSamples(HMC5883L_SAMPLES_8);
    }
   else if(compass.isQMC()){
        compass.setRange(QMC5883_RANGE_2GA);
        compass.setMeasurementMode(QMC5883_CONTINOUS); 
        compass.setDataRate(QMC5883_DATARATE_50HZ);
        compass.setSamples(QMC5883_SAMPLES_8);
   }
  
  //Startup execution ***********************************************************************************
  Serial.println("Startup Begining ");
  Startup();
  Serial.println("Startup OK");
  hc06.println("Robot Ready to Start");
  
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Robot Ready");
  
  delay(1000);
}

//********************************************************************************************************
// Main Loop

void loop()
{ 
  //getGPS();
  bluetooth();
  Ping();
  //Serial.println(gps.satellites());
  //gpsInfo();
  //getCompass();
}
