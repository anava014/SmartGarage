#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <EEPROM.h>
#include <math.h>

#define RXPIN 10
#define TXPIN 11

#define TERMBAUD  9600
#define GPSBAUD  4800

#define CALIBRATION_OFFSET 60
#define SYSTEM_STANDBY_ADDRESS 0

int BUTTON_PIN = 13;

TinyGPS gps;
SoftwareSerial uart_gps(TXPIN, RXPIN);
SoftwareSerial bluetooth(8,9);

int RED = 7;
int GREEN = 6;
int BLUE = 5;

unsigned char systemStandby = 0;
unsigned char debugMode = 1;

int initialized = 0;

double homeLatitude = 33.98069; //180 Shady Grove Dr
double homeLongitude = -117.31571;

void setSystemStandbyOn(){
  EEPROM.write(SYSTEM_STANDBY_ADDRESS, 1);
}

void setSystemStandbyOff(){
  EEPROM.write(SYSTEM_STANDBY_ADDRESS, 0);
}

unsigned char readSystemStandby(){
  return EEPROM.read(SYSTEM_STANDBY_ADDRESS); 
}

void ledON(char color){
  if(color == 'R'){
    digitalWrite(RED, HIGH); digitalWrite(GREEN, LOW); digitalWrite(BLUE, LOW);
  }
  else if(color == 'G'){
    digitalWrite(RED, LOW); digitalWrite(GREEN, HIGH); digitalWrite(BLUE, LOW);
  }
  else if(color == 'B'){
    digitalWrite(RED, LOW); digitalWrite(GREEN, LOW); digitalWrite(BLUE, HIGH);
  }
  else if(color == 'P'){
    digitalWrite(RED, HIGH); digitalWrite(GREEN, LOW); digitalWrite(BLUE, HIGH);
  }
  else{
    digitalWrite(RED, LOW); digitalWrite(GREEN, LOW); digitalWrite(BLUE, LOW);
  }
}

float distanceFromLocation(float latitude, float longitude){
  double latitudeD = latitude;
  double longitudeD = longitude;
  double x_placeHolder = pow((homeLatitude - latitudeD), 2); 
  double y_placeHolder = pow((homeLongitude - longitudeD), 2);
  
  double dist = sqrt(x_placeHolder + y_placeHolder);
  
  float distanceConverted = dist * 64.4211974984;
  float toFeet = (distanceConverted * 5280) - CALIBRATION_OFFSET;
  return toFeet;
}

void getgps(TinyGPS &gps)
{
  if(systemStandby)
  {
    ledON('P');
    for(int i = 0; i < 10; ++i)
    delay(1000);
  }
  else
    ledON('G');
      
  float latitude, longitude, distanceFromHome;
  gps.f_get_position(&latitude, &longitude);
  
  if(debugMode){
    Serial.print("Lat/Long: "); 
    Serial.print(latitude,5); 
    Serial.print(", "); 
    Serial.println(longitude,5);
  }
    
  float distance = distanceFromLocation(latitude, longitude);
  
  if(debugMode){
    Serial.print("Distance: ");
    Serial.println(distance);
  }
  
  if(distance < 200 && systemStandby == 0){
      if(debugMode)
        //Serial.println("You are within 150 Ft!");
      systemStandby = 1;
      setSystemStandbyOn();
      bluetooth.println("1");
      ledON('B');
      for(int i = 0; i < 30; ++i)
        delay(1000);
      ledON('F');
  }
  else if(distance > 150 && systemStandby == 1){
    systemStandby = 0;
    setSystemStandbyOff();
  }
  else if(distance > 150 && systemStandby == 0){
    }
  else if(distance < 150 && systemStandby == 1){
    ledON('F');
    delay(250);
    ledON('P');
    delay(250);
    ledON('F');
    delay(250);
    ledON('P');
    delay(250);
    ledON('F');
    delay(250);
  }
}

void setup()
{
  Serial.begin(TERMBAUD);
  delay(1000);
  bluetooth.begin(9600);
  delay(1000);
  uart_gps.begin(GPSBAUD);
  delay(1000);
  
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH); // pull-up
  
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
  
  systemStandby = readSystemStandby();
} 

int handle_button()
{
  int button_pressed = !digitalRead(BUTTON_PIN); // pin low -> pressed
  return button_pressed;
}

void loop()
{
  if(handle_button()){
    bluetooth.println("1");
    while(handle_button()) {}
  }
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    ledON('R');
    while (uart_gps.available())
    {
      char c = uart_gps.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {

    getgps(gps);
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}
