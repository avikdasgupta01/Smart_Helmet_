
//// NEW EDIT ////
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Avik"
#define WIFI_PASSWORD "mnbvc123456"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBnPSTxzWnXmOfkC1t3eikpw8oxy-NAtOQ"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://smart-helmet-23b25-default-rtdb.asia-southeast1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

// MPU OBJECT
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <MPU6050.h>
#include <Wire.h>
float previousValueX = 0.0;
float previousValueY = 0.0;
float previousValueZ = 0.0;
MPU6050 m_p_u;
Adafruit_MPU6050 mpu;
//


//GPS OBJECT

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial SerialGPS(1, 3); //6-> tx 7-> rx
float Latitude , Longitude;
int year , month , date, hour , minute , second;
String DateString , TimeString , LatitudeString , LongitudeString;

//
#include <OneWire.h>
#include <DallasTemperature.h> 
const int oneWireBus = 0;// d3
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

    Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");

    //Sending Temperature
    if (Firebase.RTDB.setFloat(&fbdo, "Data/Temperature in C", temperatureC)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  //mpu shinanigans
  sensors_event_t a, g, temp1;
  mpu.getEvent(&a, &g, &temp1);

  float tempf = m_p_u.getTemperature();
  float tempmpu = float(tempf + 521)/340 + 35.0;

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  //Serial.println(" Temp From MPU= ",tempmpu);

  float sensorValueX = a.acceleration.x;
  float sensorValueY = a.acceleration.y;
  float sensorValueZ = a.acceleration.z;
  
  //Sending MPU DATA
  //Sending Acc_X
    if (Firebase.RTDB.setFloat(&fbdo, "Data/ACC_X/", a.acceleration.x)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    //Sending MPU DATA
    //Sending Acc_Y
    if (Firebase.RTDB.setFloat(&fbdo, "Data/ACC_Y/", a.acceleration.y)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    //Sending MPU DATA
    //Sending Acc_Z
    if (Firebase.RTDB.setFloat(&fbdo, "Data/ACC_Z/", a.acceleration.z)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

  if (abs(sensorValueX -  previousValueX) > 5 || abs(sensorValueY -  previousValueY) > 5 || abs(sensorValueZ -  previousValueZ) > 5|| temperatureC>35) {
      Firebase.RTDB.setString(&fbdo, "Data/EMERGENCY ALERT/", "ACCIDENT- TEMPERATURE AND CRASH BOTH DETECTED"); //fatal accident
    }
    else {
      Firebase.RTDB.setString(&fbdo, "Data/EMERGENCY ALERT/", "NORMAL");
    }

    delay(1000);
    if(temperatureC>35){
      Firebase.RTDB.setString(&fbdo, "Data/TEMPERATURE ALERT/", "HIGH TEMPERATURE DETECTED"); //High Temperature
    }
    else {
      Firebase.RTDB.setString(&fbdo, "Data/TEMPERATURE ALERT/", "NORMAL");
    }
    if(abs(sensorValueX -  previousValueX) > 5 || abs(sensorValueY -  previousValueY) > 5 || abs(sensorValueZ -  previousValueZ) > 5){
      Firebase.RTDB.setString(&fbdo, "Data/ACCIDENT ALERT/", "CRASH DETECTED"); //High Temperature
    }
    else {
      Firebase.RTDB.setString(&fbdo, "Data/ACCIDENT ALERT/", "NORMAL");
    }

    previousValueX = sensorValueX;
    previousValueY = sensorValueY;
    previousValueZ = sensorValueZ;


    //gps shinanigans
    while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude , 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude , 6);
      }

      if (gps.date.isValid())
      {
        DateString = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
        DateString = '0';
        DateString += String(date);

        DateString += " / ";

        if (month < 10)
        DateString += '0';
        DateString += String(month);
        DateString += " / ";

        if (year < 10)
        DateString += '0';
        DateString += String(year);
      }

      if (gps.time.isValid())
      {
        TimeString = "";
        hour = gps.time.hour()+ 5; //adjust UTC
        minute = gps.time.minute();
        second = gps.time.second();
    
        if (hour < 10)
        TimeString = '0';
        TimeString += String(hour);
        TimeString += " : ";

        if (minute < 10)
        TimeString += '0';
        TimeString += String(minute);
        TimeString += " : ";

        if (second < 10)
        TimeString += '0';
        TimeString += String(second);
      }

    }

    String gps= "http://maps.google.com/maps?q=";
    gps+=LatitudeString;
    gps+=",";
    gps+=LongitudeString;

    //Send GPS Location
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setString(&fbdo, "Data/GPS LINK/", gps)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    count++;
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "test/temperature", 0.01 + random(0,100))){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }

  
}