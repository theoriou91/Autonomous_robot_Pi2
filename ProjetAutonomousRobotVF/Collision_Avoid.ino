#include <LiquidCrystal.h>
LiquidCrystal lcd(9,8,5,4,3,2);// for the LCD screen
 
// Ping Sensor for Collision Avoidance

boolean pingOn = true;

const byte TRIGGER_PIN = 36; // Broche TRIGGER Jaune
const byte ECHO_PIN = 37;    // Broche ECHO Vert

const unsigned long MEASURE_TIMEOUT = 25000UL;

const float SOUND_SPEED = 340.0 / 1000;


//**********************************************************************************************

void Ping()               //returns "Crash" if the distance <= 25cm
{ 
  if(pingOn){
    Serial.begin(115200);
    
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
 
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
  
  float distance_mm = measure / 2.0 * SOUND_SPEED;

  boolean crash = false;
  if (distance_mm <= 250)
  {
    crash = true;
  }
  
  if (crash){
    Stop();
    Serial.println("Obstacle - Robot Stoped");
    hc06.println("Obstacle - Robot Stoped");
    lcd.setCursor(4,1);
    lcd.print("Obstacle");
    delay(200);
    lcd.setCursor(4,1);
    lcd.print("         ");

  }
  else{}
  delay(200);
  }
  else{delay(200);}
  
  
}

//******************************************************************************************************

void pingToggle()                             // Turns Collision avoidance on/ off
 {
  if (pingOn == true) {
    pingOn = false;
    Serial.println("Collision Avoidance OFF");
    Serial1.print("Collision Avoidance OFF");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Collision Sensor");
    lcd.setCursor(6,1);
    lcd.print("OFF");
  }
    else if (pingOn == false) {
    pingOn = true;
    Serial.println("Collision Avoidance ON");
    Serial1.print("Collision Avoidance ON");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Collision Sensor");
    lcd.setCursor(7,1);
    lcd.print("ON");
  }
  
 }
