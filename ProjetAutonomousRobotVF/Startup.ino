
void Startup()
{
  Serial.println("Searching for Satellites "); 
  ss.begin(9600);
  Serial.println(gps.satellites());
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Searching for");
  lcd.setCursor(3,1);
  lcd.print("Satellites");
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
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Satellites");
  lcd.setCursor(2,1);
  lcd.print("Acquired");
  
  wpCount = 0;                                 // zero waypoint counter
  ac = 0;                                      // zero array counter
  setWaypoint();                             // set intial waypoint to current location
  Serial.println("HomePoint set");
  delay(500);    
}
