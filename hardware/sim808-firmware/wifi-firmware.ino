/**
 * Tested : 15 Januari 2021
 * Author : Lasida Azis
 * Source : https://github.com/gsampallo/esp32cam-gdrive
 * Thanks to Gsampallo
 * 
 * This script will give you output base64image with urlencode format
 * you can decode using urldecode and pase to base64 to imaage, and you will get
 * appearance from your image
 * 
 * VGA Size Image : 21KB / 21.189 bytes
 * WIFI and Camera Probem
 * Currrent Drop, Get Camera Before WIFI
 */
#include <Arduino.h>
#include <ArduinoJson.h>
#include "time.h"
#include <TimeLib.h>
#include <String.h>

#include "esp_camera.h"
#include "esp_timer.h"

#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems

//Analog Driver
#include "driver/adc.h"
#include "driver/rtc_io.h"

// Helper
#include "Base64.h"
#include "Helper.h"

//Connection
//#include "BluetoothSerial.h"
#include <WiFi.h>
#include <HTTPClient.h>

//GPS
#include "types.h"
#include "TinyGPS++.h"

//Camera Type
#define CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const char* ssid = "artemis";
const char* password = "lasida123";

//GPS GPIO
#define RXD2 14
#define TXD2 15

//LED & VIBRATION GPIO
#define GPIO_LED 12
#define BUTTON_PIN_BITMASK 0x4
#define GPIO_VIBRATION 2
#define GPIO_FLASH 4

// Object Intialize
//BluetoothSerial SerialBT;
WiFiClient client;
TinyGPSPlus gps;
HardwareSerial SerialGPS(1);
HTTPClient http;

// Set Flag Internet Connected
uint32_t chipid;  

bool status_camera = false;
bool status_gps = false;
bool device_status_online = false;
bool device_status_capture = false;

// GPS and Mode Temp
double device_lat;
double device_long;
double device_alt;
String device_mode;
int device_battery;

//Post Temp
char jsonVision[28000];
char jsonStatus[100];

//DateTime
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 3600;

char device_timenow[20];
char device_datenow[30];
char device_datetime[40];

// Sleep Variable
#define uS_TO_S_FACTOR 1000000ULL
unsigned long times;
unsigned long timeToSleep;

// RTC Data
RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR unsigned long uptime_seconds = 0;
unsigned long local_time_seconds;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  int try_connect = 0;
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
    Serial.print(".");
    if( try_connect > 10 ){
      ESP.restart();
    }
    try_connect++;
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}


//============================== SETUP ============================== //
void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 

  //The chip ID is essentially its MAC address(length: 6 bytes).
  chipid = ESP.getEfuseMac();
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(GPIO_VIBRATION, INPUT_PULLDOWN); 

  // ----- LED BLINK 10 Times ----- //
  ledcSetup(0, 5000, 13);
  ledcAttachPin(GPIO_LED, 0);
  indicator_fast_blink( 6 );
  pinMode(GPIO_FLASH, OUTPUT);

  // --> Connecting WIFI

  WiFi.disconnect(true); 
  delay(1000);

  initWiFi();

  Serial.println();
  Serial.println();
  Serial.println("Wait for WiFi... ");
  
  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);   

  // --> Setup Camera
  setupCamera();
  
  // --> Setup GPS
  delay(500);
  setupGPS();

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot Number: " + String(bootCount));
  Serial.println("Uptime : " + String(uptime_seconds));

  // --> Setup Battery
  delay(500);
  getBattery();

  //init and get the time -> Need Connection
  delay(1000);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  deviceNetworkServer();

  Serial.println("ESP32 :: " + String(chipid) +" Setup Done !!!");
  Serial.println('\n');

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  //Wake Up using Trigger | Vibration
  //print_GPIO_wake_up();
  //esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);
 
}


//============================== LOOP ============================== //
int startFlag;
unsigned long startTime = 0;
unsigned long previousTime = 0;
int duration = 100;

void loop(){
  // Log RunTime
  times = millis();
  int tseconds = times / 1000;
  local_time_seconds = tseconds % 60;
  
  Serial.println("Runtime : " + String(local_time_seconds) );
  Serial.println("Uptime : " + String(uptime_seconds) );
  
  //--------------------------------- GPS Log ---------------------------------//
  
  // Online for 3 minutes and status gps false;
  if (millis() > 180000 && status_gps == false ){
    Serial.println("3 Minutes Passed and GPS Failure...");
    getCoordinat();
  }
  
  //while (Serial2.available() >0) {
  //   gps.encode(Serial2.read());
  //}
  //if (millis() > 5000 && gps.charsProcessed() < 10)
  //{
  //  Serial.println(F("tidak ada module GPS, cek wiringmu!"));
  //  while(true);
  //}

  //--------------------------------- Vibration Reading ---------------------------------//

  // Vibration True and Flag False
  //  if ((digitalRead(GPIO_VIBRATION) == 1) && (startFlag == 0) ){ // start new time sequence
  //    startFlag = 1;
  //    startTime = millis();
  //    previousTime = startTime;
  //  }
  //
  //  // Vibration Check For 5s Continue
  //  if ( (startFlag == 1 ) && ((millis() - previousTime) >= duration) ) {  // duration = 100, sample 10 times a second
  //    previousTime = previousTime + duration;
  //    if ((millis() - startTime ) <= 5000){ // still waiting it out
  //      if (digitalRead(GPIO_VIBRATION) == 0){
  //        startFlag = 0;
  //      }else{
  //        Serial.println( "Vibration" );
  //        }
  //    }
  //    
  //    if ((millis() - startTime) > 5000){ // Reading for 5s
  //      if (digitalRead(GPIO_VIBRATION) == 0){
  //        startFlag = 0;
  //        device_mode = "charge";
  //      } else{  // made it!
  //        device_mode = "transport";
  //        Serial.println( "Mode :: Transport" );
  //        startFlag = 0;
  //      }
  //    }
  //    // do other stuff while time is passing
  //  }
  
  //--------------------------- Sending Status Online -------------------------//

  // Sending Status Online to Server  
  //  if( ! device_status_online ){
  //      if( sendStatus( "Online" ) ){
  //          device_status_online = true;
  //      }
  //  }
  //  sendStatus("Online");

  //------------------------------ Checking Time -----------------------------//

  if( !status_camera ){
      indicator_error();
      ESP.restart();
  }else{
    // timeToSleep = getTimeLeft( timetoDecimal(device_timenow));
    // Serial.print( "Time to Decimal : " ); Serial.print( timeToSleep ); Serial.println( "s" ); 
    timeToSleep = 600; // 5 minutes
    
    if( getCameraPicture() ){
      setupSleep(timeToSleep);
    }else{
      getCameraPicture();
    }
  }

  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  delay(1000);
}

//--------------------------------- Sleeping Setup ---------------------------------//
tmElements_t tm;
void setupSleep( int timeSleep ){
  // Ambil Sisa Detik untuk TIdur dari Waktu Sekarang
  char* monthList[12] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember" };
  Serial.print( "Time to Sleep : " ); Serial.print( timeSleep ); Serial.println( "s" ); 
  //sendStatus( "Sleep(" + String(timeSleep) + "s)" ); 
  
  time_t nextMakeTime;
  int ttS = timeSleep;
  uint32_t rem = ttS%3600;
  uint32_t addHour = ttS/3600;
  uint32_t addMinute = rem/60;
  uint32_t addSecond = rem%60;

  tm.Hour = getValue( device_timenow, ':', 0 ).toInt() + addHour;
  tm.Minute = getValue( device_timenow, ':', 1 ).toInt() + addMinute;
  tm.Second = 0 + addSecond;
  tm.Day = getValue( device_datenow, '/', 2 ).toInt();
  tm.Month = getValue( device_datenow, '/', 1 ).toInt();
  tm.Year = getValue( device_datenow, '/', 0 ).toInt() - 1970; // offset from 1970;
  nextMakeTime = makeTime(tm); // convert time elements into time_t

  Serial.print("Wake Up On : ");   
  Serial.print(day(nextMakeTime));   Serial.print( " " ); Serial.print( monthList[month(nextMakeTime) - 1]); Serial.print( " " ); Serial.print(year(nextMakeTime));   
  Serial.print( " @ " ); Serial.print(hour(nextMakeTime)); Serial.print( ":" ); Serial.print(minute(nextMakeTime)); Serial.print( ":" ); Serial.println(second(nextMakeTime));  
  
  uptime_seconds += local_time_seconds;
  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  esp_sleep_enable_timer_wakeup(timeSleep * uS_TO_S_FACTOR);
  
  Serial.println("SP32 to sleep for  " + String(timeSleep) + " Seconds");
  Serial.println("Sleep : zZZZzzZZZZzzzZZZ");
  
  indicator_fast_blink( 3 );
  Serial.flush(); 
  esp_deep_sleep_start();
}

//--------------------------------- Battery ---------------------------------//
void getBattery()
{
  int batt_raw;
  adc2_config_channel_atten( ADC2_CHANNEL_4, ADC_ATTEN_DB_11  );
  esp_err_t r = adc2_get_raw( ADC2_CHANNEL_4, ADC_WIDTH_12Bit, &batt_raw);

  if ( r == ESP_OK ) {
    device_battery = calcBatt( batt_raw );
    Serial.print( "Battery Capacity :: " ); Serial.print( device_battery ); Serial.println( "%" );
  } else if ( r == ESP_ERR_TIMEOUT ) {
    Serial.println("ADC2 used by Wi-Fi.\n");
  }
}

//--------------------------------- GPS ---------------------------------//
void setupGPS(){
  Serial.println( "GPS :: Checking...." );
  getCoordinat();
}

void getCoordinat(){
  while (Serial2.available() > 0 ) {
     gps.encode(Serial2.read());
  }

  if( gps.location.lat() && gps.location.lng() ){
    status_gps = true;
    device_lat = gps.location.lat();
    device_long = gps.location.lng();
    device_alt = gps.altitude.meters();
    Serial.print("GPS :: Lat = ");  Serial.println( device_lat, 6);
    Serial.print("GPS :: Long = "); Serial.println( device_long, 6);
    Serial.print("GPS :: Alt = ");  Serial.println( device_alt );
    Serial.println( "GPS :: OK !!!" );
  }else{
    status_gps = false;
    Serial.println( "GPS :: Low Signal !!!" );
  }
}

//--------------------------------- NetworkTime ---------------------------------//
void deviceNetworkServer(){
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    delay(1000);
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  strftime(device_datenow,30, "%Y/%m/%d", &timeinfo); // 2020-01-25
  strftime(device_timenow,20, "%H:%M:%S", &timeinfo); // 10:24:03
  strftime(device_datetime,40, "%Y/%m/%d, %H:%M:%S", &timeinfo);

  Serial.print("NetworkTime :: DateTime = ");
  Serial.println(device_datetime);
  
  Serial.print("NetworkTime :: Date = ");
  Serial.println(device_datenow);
  
  Serial.print("NetworkTime :: Time = ");
  Serial.println(device_timenow);
}

String deviceTimeNow(){
  return device_timenow;
}

String deviceDateNow(){
  return device_datenow;
}

//------------------------------- Device Status -------------------------------//
//bool sendStatus( String statue ){
//  uptime_seconds += local_time_seconds;
//  Serial.println("POST :: Device Status Online");
//  DynamicJsonDocument doc(100);
//  doc["chip"] = String(chipid);
//  doc["status"] = statue; 
//  doc["batt"] = device_battery;
//  doc["runtime"] = String(uptime_seconds);
//  serializeJson(doc, jsonStatus);  
//  bool rstatus = HTTP_POST_WIFI( "http://ecov.ap-1.evennode.com/v1/container/status", jsonStatus );
//}

//--------------------------------- Camera ---------------------------------//
void setupCamera(){
  Serial.print( "Camera :: Setup...." );
  
  // Camera Config
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA; // 640x480
    config.jpeg_quality = 10;  
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf(".... Failed 0x%x", err); Serial.println(" ");
    delay(1000);
    ESP.restart();
  }else{
    status_camera = true;
    Serial.println( ".... OK " );
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_vflip(s, 1); // 0 = disable , 1 = enable
}

bool getCameraPicture(){
  Serial.print("CAM :: Take Photo...");
  
  // Flash ON
  digitalWrite(GPIO_FLASH, HIGH);
  delay(200);
  digitalWrite(GPIO_FLASH, LOW);
  delay(500);
  
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.print("... Camera capture failed");
    delay(1000);
    ESP.restart();
  }
  Serial.println("... OK");

  // Encode Base64String
  Serial.print("CAM :: Encode to Base64String...");
  char *input = (char *)fb->buf;
  char output[base64_enc_len(3)];
  String base64Image = "";
  for (int i=0;i<fb->len;i++) {
    base64_encode(output, (input++), 3);
    if (i%3==0) base64Image += urlencode(String(output));
  }
  esp_camera_fb_return(fb);
  Serial.println("... OK");
//  Serial.println(base64Image);

  // Populate JSON
  Serial.print("ESP32 :: Generating Payload...");
  using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;
  SpiRamJsonDocument doc(1048576);
  doc["chip"] = String(chipid);
  doc["lat"]  = device_lat;
  doc["long"] = device_long;
  doc["batt"] = int(device_battery);
  doc["mode"] = "charge";
  doc["vision"] = base64Image;
  Serial.print("Length Image : "); Serial.println( base64Image.length());
  jsonVision[base64Image.length() + 500];
  serializeJson(doc, jsonVision);
  
  Serial.println("... OK");


//  
  // RSSI Meter
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

//  Serial.println("PSRAM found: " + String(psramFound()));
  Serial.print("Total heap: ");
  Serial.println(ESP.getHeapSize());
  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Total PSRAM: ");
  Serial.println(ESP.getPsramSize());
  Serial.print("Free PSRAM: ");
  Serial.println(ESP.getFreePsram());
  
  // Sending Payload
  Serial.print("ESP32 :: Sending Payload...");
  bool rstatus = HTTP_POST_WIFI( "http://wastecontainer.ap-1.evennode.com/v1/container/data", jsonVision );
  if( rstatus ){
    Serial.print("....OK");
    delay(1000);
    return true;
  }else{
    Serial.print("....Failed");
    return false;
  }

  //Split Base64Image
  //Serial.println( jsonVision );
  //int Index;
  //for (Index = 0; Index < 23000; Index = Index+100) {
  //  Serial.println(jsonVision.substring(Index, Index+100));
  //}
}

//--------------------------------- WIFI HTTP POST ---------------------------------//
bool HTTP_POST_WIFI( char* ENDPOINTS, char* JsonDoc)
{   
  // WIfi Connected -> Send Data to Server
  if(WiFi.status()== WL_CONNECTED){
    unsigned long start = micros();
   
    http.setTimeout(45000);
    http.begin( ENDPOINTS );
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Content-Length", String(sizeof(jsonVision)) );
    int httpCode = http.POST(JsonDoc);;
    memset(jsonVision, 0, sizeof(jsonVision));
    
    if( httpCode > 0 ){
      String response = http.getString(); 
      Serial.println(httpCode); 
      Serial.println(response);
      
      if( httpCode == 200 ){
         return true;
      }else{
         return false;
      }
    }else{
      Serial.print("Error on sending POST: ");
      Serial.println(httpCode);
      return false;

      WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
      WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
      WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED); 
    }

    http.end();
    
    unsigned long end = micros();
    unsigned long delta = end - start;
    Serial.print( "WIFI : Sent Time :: "); Serial.print( int(delta / 1000 / 60 / 60)); Serial.println("s");
  }else{
    Serial.println("Error in WiFi connection");   
    return false;
  }
}
