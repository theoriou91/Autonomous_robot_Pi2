#include "Vector.h"
#include <SoftwareSerial.h>

//GPS *******************************************************************
float flat, flon;

//COMPASS ***************************************************************
void calibrateCompass()                                           // Experimental Use Only to Calibrate Magnetometer/ Compass
{
  int minX = 0;
  int maxX = 0;
  int minY = 0;
  int maxY = 0;
  int offX = 0;
  int offY = 0; 

  for (int i=1000; i >= 1; i--) 
  {
  Vector mag = compass.readRaw();                                 // Read compass data
  
  // Determine Min / Max values
  if (mag.XAxis < minX) minX = mag.XAxis;
  if (mag.XAxis > maxX) maxX = mag.XAxis;
  if (mag.YAxis < minY) minY = mag.YAxis;
  if (mag.YAxis > maxY) maxY = mag.YAxis;
  
  offX = (maxX + minX)/2;                                         // Calculate offsets
  offY = (maxY + minY)/2;
  
  delay(10);
  //Serial.print("Compass X & Y offset: ");
  //Serial.print(offX);
  //Serial.print(" ");
  //Serial.print(offY);
  //Serial.print("\n");
  
  }                                                               // end of for loop
  
  Stop();

  Serial1.print("Compass X & Y offset: ");
  Serial1.print(offX);
  Serial1.print(" ");
  Serial1.print(offY);
  Serial.print("\n");
  //compass.setOffset(offX,offY);                                  // Set calibration offset
}
 
 // ************************************************************************************************************************************************* 

void getGPS()                             // print nb_satellites / latitude / longitude
{
  float flat, flon;
  ss.begin(9600);
  //print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
  
  gps.f_get_position(&flat, &flon);
  //print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  //print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  //Serial.println();
  
  ss.end();
  delay(500);  
}

// *************************************************************************************************************************************************
 
void setWaypoint()                                            // Set up to 5 GPS waypoints
{
if ((wpCount >= 0) && (wpCount < 50))
if (wpCount >= 0)
  {
    Serial.println("Searching for Satellites "); 
    ss.begin(9600);
    Serial.println(gps.satellites());
    while ( gps.satellites() == 255 )                         // Wait until x number of satellites are acquired before starting main loop
    {
      //gpsInfo();
      gps.f_get_position(&flat, &flon);
      nb_sats = gps.satellites();
      Serial.print("nb_sats = ");
      Serial.println(nb_sats);
      delay(50);
    }    
    
    Serial.print("GPS Waypoint ");
    Serial.print(wpCount + 1);
    Serial.println(" Set ");
                                 
    Home_LATarray[ac] = flat;       // store waypoint in an array   
    Home_LONarray[ac] = flon;       // store waypoint in an array   
                                                              
    Serial.print("Waypoint #1: ");
    Serial.print(Home_LATarray[0],6);
    Serial.print(" ");
    Serial.println(Home_LONarray[0],6);
    
    Serial.print("Waypoint #2: ");
    Serial.print(Home_LATarray[1],6);
    Serial.print(" ");
    Serial.println(Home_LONarray[1],6);
    
    Serial.print("Waypoint #3: ");
    Serial.print(Home_LATarray[2],6);
    Serial.print(" ");
    Serial.println(Home_LONarray[2],6);
    
    Serial.print("Waypoint #4: ");
    Serial.print(Home_LATarray[3],6);
    Serial.print(" ");
    Serial.println(Home_LONarray[3],6);
    
    Serial.print("Waypoint #5: ");
    Serial.print(Home_LATarray[4],6);
    Serial.print(" ");
    Serial.println(Home_LONarray[4],6);

    wpCount++;           // increment waypoint counter
    ac++;                // increment array counter
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("WayPoint");
    lcd.setCursor(10,0);
    lcd.print(wpCount);
    lcd.setCursor(12,0);
    lcd.print("Set");    
        
  }         
  else {Serial.print("Waypoints Full");}
}

// ************************************************************************************************************************************************* 

void clearWaypoints()
{
   memset(Home_LATarray, 0, sizeof(Home_LATarray));             // clear the array
   memset(Home_LONarray, 0, sizeof(Home_LONarray));             // clear the array
   wpCount = 0;                                                 // reset increment counter to 0
   ac = 0;
   
   Serial.println("GPS Waypoints Cleared");                      // display waypoints cleared
   Serial.print("Waypoint #1: ");
    
    Serial.print(Home_LATarray[0],6);
    Serial.print(" ");
    Serial.println(Home_LONarray[0],6);
    
    Serial.print("Waypoint #2: ");
    Serial.print(Home_LATarray[1],6);
    Serial.print(" ");
    Serial.println(Home_LONarray[1],6);
    
    Serial.print("Waypoint #3: ");
    Serial.print(Home_LATarray[2],6);
    Serial.print(" ");
    Serial.println(Home_LONarray[2],6);
    
    Serial.print("Waypoint #4: ");
    Serial.print(Home_LATarray[3],6);
    Serial.print(" ");
    Serial.println(Home_LONarray[3],6);
    
    Serial.print("Waypoint #5: ");
    Serial.print(Home_LATarray[4],6);
    Serial.print(" ");
    Serial.println(Home_LONarray[4],6);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WayPoints Erased");
  
}

 // *************************************************************************************************************************************************
 
void getCompass()                                               // get latest compass value
 {  
  //Serial.println("getCompass() starting");
  Vector norm = compass.readNormalize();

  float heading = atan2(norm.YAxis, norm.XAxis);
  float declinationAngle = (1.0 + (26.0 / 60.0)) / (180 / PI); // Calculated with the site https://www.magnetic-declination.com/ (we find "+1° 26' ")
  heading += declinationAngle;
  if (heading < 0){              // Correct for heading < 0deg and heading > 360deg
    heading += 2 * PI;
  }
  if (heading > 2 * PI){
    heading -= 2 * PI;
  }
  float compass_heading = heading * 180/M_PI;    // Convert to degrees

  Serial.print(" Degrees = ");
  Serial.print(compass_heading);
  Serial.println();

  delay(100);
 }

 // *************************************************************************************************************************************************

void setHeading()                                                // This procedure will set the current heading and the Heading(s) of the robot going away and returning using opposing degrees from 0 to 360;
{
   for (int i=0; i <= 5; i++)                                    // Take several readings from the compass to insure accuracy
      { 
        getCompass();                                            // get the current compass heading
      }                                               
    
    desired_heading = compass_heading;                           // set the desired heading to equal the current compass heading
    Heading_A = compass_heading;                                 // Set Heading A to current compass 
    Heading_B = compass_heading + 180;                           // Set Heading B to current compass heading + 180  

      if (Heading_B >= 360)                                      // if the heading is greater than 360 then subtract 360 from it, heading must be between 0 and 360
         {
          Heading_B = Heading_B - 360;
         }
     
    Serial.print("Compass Heading Set: "); 
    Serial.print(compass_heading);   
    Serial.print(" Degrees");
    Serial.print("desired heading");
    Serial.println(desired_heading);
    Serial.print("compass heading");
    Serial.println(compass_heading);
}

// *************************************************************************************************************************************************
 
void gpsInfo()                          // displays Satellite and GPS data to user
  {
     float flat, flon;
          
     ss.begin(9600);
     gps.f_get_position(&flat, &flon);
          
     Serial.print(" SATs ");
     print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
          
     Serial.print("Lat:");
     print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
     Serial.print(" Lon:");
     print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
     Serial.println("");
          
     delay(500);
  }


// ******************************************************************************
// Smartdelay definition 

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
// ---------------------------------------------------------------------------------
// Print Fonctions

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}


static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}
