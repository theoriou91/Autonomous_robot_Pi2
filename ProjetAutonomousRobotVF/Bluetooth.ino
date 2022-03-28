#include <SoftwareSerial.h>

#define PWM1 23
#define AIN1 27
#define AIN2 25
#define PWM2 35
#define BIN1 31
#define BIN2 33

SoftwareSerial hc06(10, 11);
String str;
int blueToothVal;
int pot;

//**************************************************************************************************************************************************

void bluetooth()
{
 //Serial.println("Lecture Bluetooth Ativée");
 //getGPS();
 hc06.begin(4800);
 
 while (hc06.available())                             // Read bluetooth commands over Serial1 // Warning: If an error with Serial1 occurs, make sure Arduino Mega 2560 is Selected
 {  
  {  
      str = hc06.readStringUntil('\n');               // str is the temporary variable for storing the last sring sent over bluetooth from Android device
      //Serial.print(str);             
  } 
    
    blueToothVal = (str.toInt());                        //  convert the string 'str' into an integer and assign it to blueToothVal
    Serial.print("BlueTooth Value : ");
    Serial.println(blueToothVal);    
     
 switch (blueToothVal) 
 {
      case 1:
        Forward();
        break;

      case 2:
        Reverse();
        break;

      case 3:         
        LeftTurn();
        Stop();
        break;
        
      case 4:
        RightTurn();
        Stop();
        break;
        
      case 5:
        Stop();
        break; 
        
     case 6:                 
        setWaypoint();
        break;
      
      case 7:        
        goWaypoint();
        break;  
      
      case 8:  
        turnAround();
        Stop();
        break;
      
      case 9:
        setHeading();
        Compass_Forward();
        break;
      
      case 10:
        setHeading();
        break; 

      case 11:
         hc06.println("GPS printing in the serial monitor ...");
         delay(3000);
         gpsInfo();
        break;
      
      case 12:  
        Serial.println("Compass Turn Right");
        CompassTurnRight();
        break;
      
      case 13:  
        Serial.println("Compass Turn Left");
        CompassTurnLeft();
        break;
        
      case 14:  
        Serial.println("Calibrate Compass");
        calibrateCompass();
        break;

      case 15:  
        pingToggle();
        break;  

      case 16:
        clearWaypoints();
        break;  

      case 17:                    // finish with waypoints
        //ac = 0;
        Serial.print("Waypoints Complete");
        break;
    
 }                                                           // end of switch case
 }                                                           // end of while loop Serial1 read
                                                             // if no data from Bluetooth 
   if (hc06.available() < 0)                                 // if an error occurs, confirm that the arduino mega board is selected in the Tools Menu
    {
     Serial.println("No Bluetooth Data ");          
    }
} // end of bluetooth procedure
