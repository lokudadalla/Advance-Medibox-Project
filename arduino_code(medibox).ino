//last one

// Medibox Project
// Name - Lokudadalla L.D.S.D
// Index Number - 210346D


// Libraries
#include <Wire.h> // I2C Library
#include <Adafruit_GFX.h> // Graphics Library
#include <Adafruit_SSD1306.h> // OLED Display Library
#include <DHTesp.h> // DHT Library
#include <WiFi.h> // WiFi Library
#include <ESP32Servo.h> //servo Library
#include <PubSubClient.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

//defining servo pins
#define servoPIN 23

//LDR pins
#define LDR_LEFT 35
#define LDR_RIGHT 34

// Defining OLED display parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_RESET -1
#define SCREEN_ADDRESS 0x3C

// NTP Server details
#define NTP_SERVER   "pool.ntp.org"
int UTC_OFFSET = 0;
int UTC_OFFSET_DST = 0;

// Declare objects
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,SCREEN_RESET);
DHTesp dhtSensor;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

Servo servoMotor;



// Define Pins
#define BUZZER 5
#define LED_1 15
#define LED_2 2
#define PB_CANCEL 32
#define PB_OK 25
#define PB_UP 26
#define PB_DOWN 33
#define DHTPIN 12

// Gllobal Variables
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long  time_now = 0;
unsigned long  time_last = 0;

bool alarm_enabled = true;
int n_alarms = 3;
int alarm_hours[] = {0,1,2};
int alarm_minutes[] = {1,10,33};
bool alarm_triggered[] = {false,false,false};

// Notes for buzzer
int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;

int notes[] = {C,D,E,F,G,A,B,C_H};

// Mode parameters
int current_mode = 0;
int max_modes = 5;
String modes[] = {"1 - Set Time Zone","2 - Set Alarm 1","3 - Set Alarm 2","4 - Set Alarm 3","5 - Disable Alarms"};
//added new set alarm 3

char tempAr[6];
char humAr[6];


bool isScheduledON = false;
//bool isScheduled = false;
unsigned long scheduledOnTime;

char side; //use to max_intensity
float Max_Intensity; // Store maximum intensity value
char intensityStrAr[6]; //  intensity Value Array

//int current_angle = 0; // Current angle of the servo

int Min_Angle = 30;
float ControllingFactor = 0.75;



void setup() {

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(PB_CANCEL, INPUT);
  pinMode(PB_OK, INPUT);
  pinMode(PB_DOWN, INPUT);
  pinMode(PB_UP, INPUT);
  pinMode(LDR_LEFT, INPUT);
  pinMode(LDR_RIGHT, INPUT);

  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  // Show initial display buffer contents on the screen -- 
  // the library initializes this with an Adafruit splash screen. 
  display.display();
  delay(2000); // Pause for 2 seconds
  
  // Clear the buffer 
  display.clearDisplay();
 

  print_line("Welcome to Medibox!", 10, 20, 2);
  display.clearDisplay();

  dhtSensor.setup(DHTPIN,DHTesp::DHT22);

  servoMotor.attach(servoPIN, 500, 2400); //setup servo

  WiFi.begin("Wokwi-GUEST", "", 6);
    while (WiFi.status() != WL_CONNECTED) {
      delay(250);
      display.clearDisplay();
      print_line("Connecting to WIFI", 0,0,2);
    }

    display.clearDisplay();
    print_line("Connected to WIFI", 0,0,2);
  

  //setup MQTT
    setupMqtt();
    display.clearDisplay();

    timeClient.begin();
    timeClient.setTimeOffset(5.5 * 3600);
    configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

    //this for LDR to anolog read 1024 values
    analogReadResolution(10);
  
  
  }  



void loop() {
  // put your main code here, to run repeatedly:

  if(!mqttClient.connected()){
    ConnectToBroker();
  }
  mqttClient.loop();

  update_time_with_check_alarm();
  if(digitalRead(PB_OK) == LOW){

    delay(200);
    go_to_menu();
  }
  
  check_tempAndHUM();

  checkSchedule();

  MAX_INTENSITY_LDR();  
  

  mqttClient.publish("ESP-temp-MEDI",tempAr);
  mqttClient.publish("ESP-hum-MEDI",humAr);

  
  update_servo_position();


}

unsigned long getTime(){
  timeClient.update();
  return timeClient.getEpochTime();
}

void checkSchedule(){                                
  if(isScheduledON){
    unsigned long currentTime = getTime();
    if (currentTime > scheduledOnTime ){
      buzzerOn(true);
      isScheduledON = false;
      mqttClient.publish("MEDI-MAIN-ON-OFF-ESP","1");
      mqttClient.publish("SCHEDULE SWITCH","0");
      Serial.println("scheduleON");

    }
  }
}

void setupMqtt(){
  mqttClient.setServer("test.mosquitto.org",1883);
  mqttClient.setCallback(receiveCallback);
}


void receiveCallback(char* topic,byte* payload, unsigned int length){

  char charPayloadAr[length];
  for (int i=0 ;i<length;i++){
    charPayloadAr[i] = (char)payload[i];
  }

  if (strcmp(topic,"MEDI-MAIN-ON-OFF")==0){
    if(charPayloadAr[0]=='1'){
      buzzerOn(true);
    }else{
      buzzerOn(false);
    }
  }else if(strcmp(topic,"SCHEDULE SWITCH")==0){
    Serial.println(charPayloadAr[0]);
    if(charPayloadAr[0] == 'N'){
      isScheduledON = false;
    }
    else {
      isScheduledON = true;
      scheduledOnTime = atol(charPayloadAr);
    } 
  }
  else if(strcmp(topic,"Min Angle Servo")==0){
     Min_Angle = atol(charPayloadAr);  //get Min angle values
    
  }
  else if(strcmp(topic,"Controlling Factor Servo")==0){
    ControllingFactor = atof(charPayloadAr);  //get controlling factor values
    
  }
  }



void buzzerOn(bool on){
  if(on){
    for(int i=0;i<n_notes;i++){
      tone(BUZZER,notes[i]);
      delay(500);
      noTone(BUZZER);
      delay(2);
  }
  }else{
    noTone(BUZZER);
  }
  
}

void ConnectToBroker(){
  while (!mqttClient.connected()){
    display.clearDisplay();
    print_line("Attempting MQTT Connection",0,0,2);
    if(mqttClient.connect("ESP32-12345645454")){
      display.clearDisplay();
      print_line("Connected",0,0,2);
      mqttClient.subscribe("MEDI-MAIN-ON-OFF");
      mqttClient.subscribe("SCHEDULE SWITCH");
      mqttClient.subscribe("Min Angle Servo");  //subscribe mqtt server to get data
      mqttClient.subscribe("Controlling Factor Servo"); //subscribe mqtt server to get data
    }
    else{ // this is for mqtt server does not connect
      display.clearDisplay();  
      print_line("Failed MQTT Connection",0,0,2);
      Serial.println(mqttClient.state());
      delay(5000);
    }   
  }
}


void print_line(String text,int column,int row,int text_size){

  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column,row); // Column,Row
  display.println(text);

  display.display();
}

void print_time_now(void){

  display.clearDisplay();
  print_line(String(days), 0, 0, 2);
  print_line(":", 20, 0, 2);
  print_line(String (hours), 30, 0, 2);
  print_line(":", 50, 0, 2);
  print_line(String(minutes),60, 0, 2);
  print_line(" :", 80, 0, 2);
  print_line(String(seconds),90, 0, 2);

}


void update_time(void){

  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char timeHour[3];
  strftime(timeHour,3,"%H",&timeinfo);
  hours = atoi(timeHour);

  char timeMinute[3];
  strftime(timeMinute,3,"%M",&timeinfo);
  minutes = atoi(timeMinute);

  char timeSecond[3];
  strftime(timeSecond,3,"%S",&timeinfo);
  seconds = atoi(timeSecond);

  char timeDay[3];
  strftime(timeDay,3,"%d",&timeinfo);
  days = atoi(timeDay);


}


void update_time_with_check_alarm(void){
  print_time_now();
  update_time();


  if(alarm_enabled == true){
    for(int i=0;i<n_alarms;i++){
      if(alarm_triggered[i] == false && alarm_hours[i] == hours && alarm_minutes[i] == minutes){
        ring_alarm();
        alarm_triggered[i] = true;
      }
    }
  }

}

void ring_alarm(void){
  display.clearDisplay();
  print_line("Medicine Time!",0,0,2);
  digitalWrite(LED_1,HIGH);

  bool break_happened = false;

  while(digitalRead(PB_CANCEL) == HIGH && break_happened == false){
    // Ring the Buzzer
    for(int i=0;i<n_notes;i++){
      if(digitalRead(PB_CANCEL) == LOW){
        delay(200);
        break_happened = true;
        break;
      }
      tone(BUZZER,notes[i]);
      delay(500);
      noTone(BUZZER);
      delay(2);
    }
  }

  digitalWrite(LED_1,LOW);
  display.clearDisplay();

}


void go_to_menu(void){
  display.clearDisplay();
  print_line("Menu",0,2,2);
  delay(1000);

  while(digitalRead(PB_CANCEL) == HIGH){

    display.clearDisplay();
    print_line(modes[current_mode],0,2,2);
    delay(1000);

    int pressed = wait_for_button_press();

    if(pressed == PB_UP){
      delay(200);
      current_mode += 1;
      current_mode = current_mode % max_modes;
    }
    else if(pressed == PB_DOWN){
      delay(200);
      current_mode -= 1;
      if(current_mode <= -1){
        current_mode = max_modes - 1;
      }
    }
    else if(pressed == PB_OK){
      delay(200);
      run_mode(current_mode);
    }
    else if(pressed == PB_CANCEL){
      delay(200);
      break;
    }

  }
}


int wait_for_button_press(){
  while(true){
    if(digitalRead(PB_UP) == LOW){
  
      delay(200);
      return PB_UP;
    }
    else if(digitalRead(PB_DOWN) == LOW){
  
      delay(200);
      return PB_DOWN;
    }
    else if(digitalRead(PB_OK) == LOW){
  
      delay(200);
      return PB_OK;
    }
    else if(digitalRead(PB_CANCEL) == LOW){
  
      delay(200);
      return PB_CANCEL;
    }
  }
}


void run_mode(int mode){
  if(mode == 0){
    set_time_zone();
  }
  else if(mode == 1 || mode == 2 || mode == 3){
    set_alarm(mode - 1);
  }
  else if(mode == 4){
    alarm_enabled = false;
  }
}


void set_time_zone(){
  int temp_Time_Zone_hour = 0; //set time zone hour to 0

  while(true){
    display.clearDisplay();
    print_line("Enter time zone hour: " + String(temp_Time_Zone_hour),0,0,2);

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_Time_Zone_hour += 1;//increment hours
        temp_Time_Zone_hour = temp_Time_Zone_hour % 24;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_Time_Zone_hour -= 1;//decreament hours
        if (temp_Time_Zone_hour<0){
          temp_Time_Zone_hour = 24 -1;
      }
      }
      else if(pressed == PB_OK){
        delay(200);
        UTC_OFFSET = temp_Time_Zone_hour * 3600;//hours convert to seconds
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  int temp_Time_Zone_minute = 0;
  //time zone minute set to 0

  while(true){
    display.clearDisplay();
    print_line("Enter time zone Minute: " + String(temp_Time_Zone_minute),0,0,2);

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_Time_Zone_minute += 15;//increament  minutes
        temp_Time_Zone_minute = temp_Time_Zone_minute % 60;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_Time_Zone_minute -= 15;//decreament minute
        if(temp_Time_Zone_minute < 0){
          temp_Time_Zone_minute = 60 - 1;
        }
      }
      else if(pressed == PB_OK){
        delay(200);
        int UTC_OFFSET_min = (temp_Time_Zone_minute * 60);//minutes convert seconds
        UTC_OFFSET += UTC_OFFSET_min; //overall offset
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  configTime(UTC_OFFSET,UTC_OFFSET_DST,NTP_SERVER);


  display.clearDisplay();
  print_line("Time Zone is set",0,0,2); // Time zone set
  delay(1000);

}


void set_alarm(int alarm){
  int temp_hour = alarm_hours[alarm];

  while(true){
    display.clearDisplay();
    print_line("Enter hour: " + String(temp_hour),0,0,2); 

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_hour += 1;
        temp_hour = temp_hour % 24;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_hour -= 1;
        if(temp_hour <= -1){
          temp_hour = 24 - 1;
        }
      }
      else if(pressed == PB_OK){
        delay(200);
        alarm_hours[alarm] = temp_hour;
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  int temp_minute = alarm_minutes[alarm];

  while(true){
    display.clearDisplay();
    print_line("Enter Minute: " + String(temp_minute),0,0,2);

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_minute += 1;
        temp_minute = temp_minute % 60;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_minute -= 1;
        if(temp_minute <= -1){
          temp_minute = 60 - 1;
        }
      }
      else if(pressed == PB_OK){
        delay(200);
        alarm_minutes[alarm] = temp_minute;
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  display.clearDisplay();
  alarm_enabled = true;
  alarm_triggered[alarm] = false;
  print_line("Alarm "+String(alarm+1)+" is set",0,0,2);
  delay(1000);
}



void check_tempAndHUM(){
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  String(data.temperature,2).toCharArray(tempAr,6);
  String(data.humidity,2).toCharArray(humAr,6);
  

  if(data.temperature > 32){
    display.clearDisplay();
    print_line(" TEMP HIGH",0,30,2);
    delay(1000);
  }
  if(data.temperature < 26){
    display.clearDisplay();
    print_line(" TEMP LOW",0,30,2);
    delay(1000);
  }
  if(data.humidity > 80){
    display.clearDisplay();
    print_line(" HUMIDITY",0,20,2);
    print_line("   HIGH",0,40,2);
    delay(1000);
  }
  if(data.humidity < 60){
    display.clearDisplay();
    print_line(" HUMIDITY",0,20,2);
    print_line("   LOW",0,40,2);
    delay(1000);
  }
}


void MAX_INTENSITY_LDR() {
    // Read the values from the LDR
    float LDR_L_val = analogRead(LDR_LEFT) / 1023.0; // Normalized LDR left
    float LDR_R_val = analogRead(LDR_RIGHT) / 1023.0; // Normalized LDR right

    Serial.println(LDR_LEFT);
    Serial.println(LDR_RIGHT);

    // Calculate intensity as 1 minus the normalized value
    float intensity_left = (1-LDR_L_val);
    float intensity_right = (1-LDR_R_val);
    
    //MAX_intensity  happens left or right
    if (intensity_left > intensity_right) {
        Max_Intensity = intensity_left;
        side = 'L';// 'L' for left
    } else {
        Max_Intensity = intensity_right;
        side = 'R';  //'R' for right
    }
    
    // Convert the maximum intensity to string and send MQTT messages
    String(Max_Intensity, 6).toCharArray(intensityStrAr, 6);
    char msg[2] = {side, '\0'}; // Create message to send the side
    
    // Publish messages to the MQTT topics
    mqttClient.publish("LEFT OR RIGHT LDR", msg);
    mqttClient.publish("Max Intensity Of LDR", intensityStrAr);
}


float calculate_servo_angle() {
    float D ;
    int offset_theta = Min_Angle;
    if(side == 'L') {
        D = 1.5;
     }else{
       D = 0.5;
     }

    //Serial.print(D);
    
    int desired_angle = D * offset_theta + (180 - offset_theta) * Max_Intensity * ControllingFactor;
    
    return min(desired_angle, 180); // Ensure the angle does not exceed 180 degrees
}

// Update the servo position to the new calculated angle
void update_servo_position() {
    int current_angle;
    int new_angle = int(calculate_servo_angle()); // Calculate the new angle
    if (new_angle != current_angle) { // Check if the angle has changed
        if (current_angle > new_angle) {
            for (int position = current_angle; position >= new_angle; position--) {
                servoMotor.write(position); // Move the servo to the new position
                delay(10);
            }
        } else {
            for (int position = current_angle; position <= new_angle; position++) {
                servoMotor.write(position); // Move the servo to the new position
                delay(10);
            }
        }
        current_angle = new_angle; // Update the current angle to the new angle
    }
}


