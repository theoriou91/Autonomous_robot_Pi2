int out=map(255,0,255,0,255); // Syntax : map(value, fromLow, fromHigh, toLow, toHigh)
#include "TinyGPS.h"

// Motor Control Section ***********************************************************

void Forward()
{
  Serial.println("Robot going Forward");
  hc06.println("Robot going Forward");
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Forward");
  lcd.setCursor(4,1);
  lcd.print(" >>>>> ");
  digitalWrite(AIN1,HIGH); //Motor A Rotate Clockwise
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); //Motor B Rotate Clockwise
  digitalWrite(BIN2,LOW);
  
  out=map(150,0,150,0,150);
  analogWrite(PWM1,out); //Speed control of Motor A
  analogWrite(PWM2,out); //Speed control of Motor B
   
}


void Reverse()
{
  Serial.println("Robot going Reverse");
  hc06.println("Robot going Reverse");
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Reverse");
  lcd.setCursor(4,1);
  lcd.print(" <<<<< ");
  digitalWrite(AIN1,LOW); //Motor A Rotate CounterClockwise
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW); //Motor B Rotate CounterClockwise
  digitalWrite(BIN2,HIGH);
  
  out=map(150,0,255,0,255);
  analogWrite(PWM1,out); //Speed control of Motor A
  analogWrite(PWM2,out); //Speed control of Motor B
  delay(500);
}


void LeftTurn()
{
  Serial.println("Robot turning Left");
  hc06.println("Robot turning Left");
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Left Turn");
  digitalWrite(AIN1,HIGH); //Motor A Rotate Clockwise
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); //Motor B Stop
  digitalWrite(BIN2,LOW);
  
  out=map(150,0,255,0,255);
  analogWrite(PWM1,out); //Speed control of Motor A
  analogWrite(PWM2,out); //Speed control of Motor B
  delay(200); 
}


void RightTurn()
{                     
  Serial.println("Robot turning Right");
  hc06.println("Robot turning Right");
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Right Turn");
  digitalWrite(AIN1,LOW); //Motor A Stop
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); //Motor B Rotate CounterClockwise
  digitalWrite(BIN2,LOW);
  
  out=map(150,0,255,0,255);
  analogWrite(PWM1,out); //Speed control of Motor A
  analogWrite(PWM2,out); //Speed control of Motor B
  delay(200);                                                                  
}

void Stop()
{ 
  Serial.println("Robot Stoped ");
  hc06.println("Robot Stoped");
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("Stop");
  digitalWrite(AIN1,LOW); //Motor A Stop
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); //Motor B Stop
  digitalWrite(BIN2,LOW);
  
  out=map(255,0,255,0,255);
  analogWrite(PWM1,out); //Speed control of Motor A
  analogWrite(PWM2,out); //Speed control of Motor B
}
// *******************************************************************
void goWaypoint()
{   
 Serial.println("Going to Waypoint");
 hc06.println("Going to Waypoint");
 lcd.clear();
 lcd.setCursor(2,0);
 lcd.print("Going to WP");

//Serial.print("Home_Latarray ");
//Serial.print(Home_LATarray[ac],6);
//Serial.print(" ");
//Serial.println(Home_LONarray[ac],6);   

//Serial1.print("Distance to Waypoint");   
//Serial1.print(Distance_To_Home);

//Serial1.print("ac= ");
//Serial1.print(ac);

 while (true)  
  {                                                                // Start of Go_Home procedure 
  bluetooth();                                                     // Run the Bluetooth procedure to see if there is any data being sent via BT
  if (blueToothVal == 5){break;}                                   // If a 'Stop' Bluetooth command is received then break from the Loop
  
  Serial.println("Searching for Satellites "); 
  ss.begin(9600);
  Serial.println(gps.satellites());
  while ( gps.satellites() == 255)                         // Wait until x number of satellites are acquired before starting main loop
  {
    gpsInfo();
    gps.f_get_position(&flat, &flon);
    nb_sats = gps.satellites();
    Serial.print("nb_sats = ");
    Serial.println(nb_sats);
    delay(50);
  } 
  Serial.println("Satellites Acquired");    
 
  Distance_To_Home = TinyGPS::distance_between(flat, flon, Home_LATarray[ac], Home_LONarray[ac]);  //Query Tiny GPS for Distance to Destination
  GPS_Course = TinyGPS::course_to(flat, flon, Home_LATarray[ac], Home_LONarray[ac]);               //Query Tiny GPS for Course to Destination   
   
    if (Home_LATarray[ac] == 0) {
      Serial1.print("End of Waypoints");
      Stop();      
      break;
      }      
    if (Distance_To_Home == 0)                                   // If the Vehicle has reached it's Destination, then Stop
        {
        Stop();                                               // Stop the robot after each waypoint is reached
        Serial.println("You have arrived!");                    // Print to Bluetooth device - "You have arrived"          
        ac++;                                                    // increment counter for next waypoint
        break;                                                   // Break from Go_Home procedure and send control back to the Void Loop 
                                                                 // go to next waypoint
        
        }   
   if ( abs(GPS_Course - compass_heading) <= 15)                  // If GPS Course and the Compass Heading are within x degrees of each other then go Forward                                                                  
                                                                  // otherwise find the shortest turn radius and turn left or right  
       {
         Forward();                                               // Go Forward
       } else 
         {                                                       
            int x = (GPS_Course - 360);                           // x = the GPS desired heading - 360
            int y = (compass_heading - (x));                      // y = the Compass heading - x
            int z = (y - 360);                                    // z = y - 360
            
            if ((z <= 180) && (z >= 0))                           // if z is less than 180 and not a negative value then turn left otherwise turn right
                  { SlowLeftTurn();  }
             else { SlowRightTurn(); }               
        }
  }                                                              // End of While Loop
}                                                                // End of Go_Home procedure


// **********************************************************************************************************************************************************************

void Forward_Meter()
{
  Serial.println("Forward");

  digitalWrite(AIN1,HIGH); //Motor A Rotate Clockwise
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); //Motor B Rotate Clockwise
  digitalWrite(BIN2,LOW);
  
  out=map(255,0,255,0,255);
  analogWrite(PWM1,out); //Speed control of Motor A
  analogWrite(PWM2,out); //Speed control of Motor B

  delay(500);
}

// **********************************************************************************************************************************************************************


void SlowLeftTurn()
{
  Serial.println("Left");

  digitalWrite(AIN1,HIGH); //Motor A Rotate Clockwise
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); //Motor B Stop
  digitalWrite(BIN2,LOW);
  
  out=map(255,0,255,0,255);
  analogWrite(PWM1,out); //Speed control of Motor A
  analogWrite(PWM2,out); //Speed control of Motor B
  delay(500); 
}

// **********************************************************************************************************************************************************************

void SlowRightTurn()
{
  Serial.println("Right");

  digitalWrite(AIN1,LOW); //Motor A Stop
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); //Motor B Rotate CounterClockwise
  digitalWrite(BIN2,LOW);
  
  out=map(255,0,255,0,255);
  analogWrite(PWM1,out); //Speed control of Motor A
  analogWrite(PWM2,out); //Speed control of Motor B
  delay(500);                                                                  
}


// **********************************************************************************************************************************************************************


// Compass Drive Section
// This Portion of code steers the Vehicle based on the compass heading
// **********************************************************************************************************************************************************************

void CompassTurnRight()                                                          // This Function Turns the car 90 degrees to the right based on the current desired heading
{
  Stop();    
  getCompass();                                                                  // get current compass heading      

  desired_heading = (desired_heading + 90);                                      // set desired_heading to plus 90 degrees 
  if (desired_heading >= 360) {desired_heading = (desired_heading - 360);}       // if the desired heading is greater than 360 then subtract 360 from it

  while ( abs(desired_heading - compass_heading) >= compass_dev)                 // If the desired heading is more than Compass Deviation in degrees from the actual compass heading then
      {                                                                          // correct direction by turning left or right

    getCompass();                                                                // Update compass heading during While Loop
    bluetooth();                                                                 // if new bluetooth value received break from loop        
    if (blueToothVal == 5){break;}                                               // If a Stop Bluetooth command ('5') is received then break from the Loop
        
    if (desired_heading >= 360) {desired_heading = (desired_heading - 360);}     // if the desired heading is greater than 360 then subtract 360 from it                                            
                                                                
    int x = (desired_heading - 359);                                             // x = the GPS desired heading - 360    
    int y = (compass_heading - (x));                                             // y = the Compass heading - x
    int z = (y - 360);                                                           // z = y - 360
            
        if ((z <= 180) && (z >= 0))                                              // if z is less than 180 and not a negative value then turn left 
            {                                                                    // otherwise turn right
              LeftTurn();                            
            } 
            else
            {
              RightTurn();        
            }  
        }    
    {
      Stop();                                                                  // Stop the Car when desired heading and compass heading match
    }
 }    


// **********************************************************************************************************************************************************************

void CompassTurnLeft()                                                           // This procedure turns the car 90 degrees to the left based on the current desired heading
{
  Stop();    
  getCompass();                                                                  // get current compass heading                                                                                  
  //desired_heading = (compass_heading - 90);                                    // set desired_heading to compass value minus 90 degrees

  desired_heading = (desired_heading - 90);                                      // set desired_heading to minus 90 degrees
  if (desired_heading <= 0) {desired_heading = (desired_heading + 360);}         // if the desired heading is greater than 360 then add 360 to it
  while ( abs(desired_heading - compass_heading) >= compass_dev)                 // If the desired heading is more than Compass Deviation in degrees from the actual compass heading then
      {                                                                          // correct direction by turning left or right
    getCompass();                                                                // Get compass heading again during While Loop
    bluetooth();                                                                 // if new bluetooth value received break from loop              
    if (blueToothVal == 5){break;}                                               // If a 'Stop' Bluetooth command is received then break from the Loop
    
    if (desired_heading >= 360) {desired_heading = (desired_heading - 360);}     // if the desired heading is greater than 360 then subtract 360 from it                                            
                                                                
    int x = (desired_heading - 359);                                             // x = the desired heading - 360    
    int y = (compass_heading - (x));                                             // y = the Compass heading - x
    int z = (y - 360);                                                           // z = y - 360
        if (z <= 180)                                                            // if z is less than 180 and not a negative value then turn left         
       // if ((z <= 180) && (z >= 0))                                             
            {                                                                    // otherwise turn right
              SlowLeftTurn();                             
            } 
            else
            {
              SlowRightTurn();              
            }  
        }    
    {
      Stop();                                                                 // Stop the Car when desired heading and compass heading match
    }
 }   

// **********************************************************************************************************************************************************************

void Compass_Forward()                                               
{ 
  Serial.println("Compass Forward");
  hc06.println("Compass Forward");         
  while (blueToothVal == 9)                                           // Go forward until Bluetooth 'Stop' command is sent

  //while (true)                                                        
   {  
    getCompass();                                                     // Update Compass Heading
    bluetooth();                                                      // Check to see if any Bluetooth commands have been sent
    if (blueToothVal == 5) {break;}                                   // If a Stop Bluetooth command ('5') is received then break from the Loop
    
    if ( abs(desired_heading - compass_heading) <= compass_dev )      // If the Desired Heading and the Compass Heading are within the compass deviation, X degrees of each other then Go Forward
                                                                      // otherwise find the shortest turn radius and turn left or right  
       {
         Forward(); 
         Ping();       
       } else 
         {    
            int x = (desired_heading - 359);                          // x = the GPS desired heading - 360
            int y = (compass_heading - (x));                          // y = the Compass heading - x
            int z = (y - 360);                                        // z = y - 360
                     
            if ((z <= 180) && (z >= 0))                               // if z is less than 180 and not a negative value then turn left 
            {                                                         // otherwise turn right
              SlowLeftTurn();
              Ping();           
            }
            else
            {
              SlowRightTurn();
              Ping(); 
            }              
        } 
 }                                                                   // End While Loop
}                                                                    // End Compass_Forward

// **********************************************************************************************************************************************************************

void turnAround()                                                   
 {                                                                  
  Serial.println("Robot Turning Around");
  hc06.println("Robot turning Around");
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("180 deg Turn");
  digitalWrite(AIN1,HIGH); //Motor A Rotate Clockwise
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); //Motor B Rotate Oposite
  digitalWrite(BIN2,HIGH);
  
  out=map(150,0,255,0,255);
  analogWrite(PWM1,out); //Speed control of Motor A
  analogWrite(PWM2,out); //Speed control of Motor B
  delay(500); 
}
