/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
// #define BLYNK_TEMPLATE_ID "TMPLuFaZPNSo"
// #define BLYNK_DEVICE_NAME "Quickstart Template"
// #define BLYNK_AUTH_TOKEN "SjToXYysGYdt2QbgljIQdSLYPhFkzqMC"

#define BLYNK_TEMPLATE_ID "TMPLs3Yuj_og"
#define BLYNK_DEVICE_NAME "Quickstart TemplateCopy"
#define BLYNK_AUTH_TOKEN "LCMK6U9u-iB_VECQQayN-0YAC38gvONk"



#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <PubSubClient.h>
#include <MQTT.h>

//trai
#define in1 32
#define in2 33
// phai
#define in3 25
#define in4 26

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
// V0 : đi tới - V1: đi lui - V3: rẽ trái - V4: rẽ phải
//l298n in1 in2 in3 in4 ~ d32 d33 d25 d26
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
// char ssid[] = "jennierubyjane";
// char pass[] = "jenniekim";

char ssid[] = "Quynh Huong";
char pass[] = "hehehehee";

// char ssid[] ="The New Zone";
// char pass[] = "123456789";

// char ssid[] ="HY";
// char pass[] = "12345678";

int mode=1;
// mosquitto -v to run mqtt server, set allow anonym bằng true để cho phép tất cả các client truy cập, listener port equals 1183 : D
// Cảm biến
int sensor_left = 18; //Cảm biến trái
int sensor_right = 19; //Cảm biến phải
// Giá trị cảm biến
int value_sensor_left, value_sensor_right;

// 172.20.10.3 
const char* server = "172.20.10.8"; // mqtt broker ip

//const char* server = "localhost"; 

//const char* server = "localhost";
const char* topic = "test/topic";
const char* clientName = "nhiday";

 
int channel = 1;
int hz = 50;
int depth = 16;
 
WiFiClient wifiClient;
PubSubClient client(wifiClient);

 
void wifiConnect() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  //WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }
 
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
}

void mqttReConnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientName)) {
      Serial.println("connected");
      client.setBufferSize(4096);
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


// motor trái

void motor_left(byte dir) 
{ 
   if (dir == 0) // motor trái quay tới
   {  
     digitalWrite(in1, HIGH);
     digitalWrite(in2, LOW);

   }
   else if (dir == 1) // motor trái quay lùi
   {
     digitalWrite(in2, HIGH);
     digitalWrite(in1, LOW);
   }
   else if (dir == 2)// motor trái đứng yên
   {
    digitalWrite(in1, LOW);
    digitalWrite(in2,LOW);
    }
}
// motor phải
void motor_right(byte dir) 
{ 
   if (dir == 0) // motor phải quay tới 
   { 
    digitalWrite(in4, LOW);
    digitalWrite(in3, HIGH);
   }
   else if (dir == 1)//motor phải quay lùi
   { 
     digitalWrite(in3, LOW);
     digitalWrite(in4, HIGH);
   }
   else if (dir ==2)//motor phải dứng lùi
   {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    }
}


void goStraightForwards()
{
 // Serial.println("motor di thang");
  
  motor_right(0);
  motor_left(0); 
}
void goBackwards()
{
  //Serial.println("motor lui");
  
  motor_right(1);
  motor_left(1);
}
void turnLeft()
{
  //Serial.println("motor re trai");
  
  motor_right(0);
  motor_left(1);
}
void turnRight()
{
  //Serial.println("motor re phai");
  
  motor_right(1);
  motor_left(0);
}

void stop()
{
  //Serial.println("motor dung");
  motor_right(2);
  motor_left(2);
}
void controlBySensor(int left_value, int right_value)
{
  if(left_value == 1 && right_value == 1)
  {
    goStraightForwards();
    Blynk.virtualWrite(V8, "di thang");
  }
  else if(left_value == 0 && right_value == 0)
  {
    goBackwards();
    Blynk.virtualWrite(V8, "lui");
  }
  else if(left_value == 1 && right_value == 0)
  {
    turnLeft();
    Blynk.virtualWrite(V8, "trai");
  }
  else if(left_value == 0 && right_value == 1)
  {
    turnRight();
    Blynk.virtualWrite(V8, "phai");
  }
}
void sensor()
{
  value_sensor_right = digitalRead(sensor_right);
  value_sensor_left = digitalRead(sensor_left);
  controlBySensor(value_sensor_right, value_sensor_left);
      
    
}


// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  
  if (value){
    Serial.println("Straight: " + (String)value);
    goStraightForwards();
  }

}

BLYNK_WRITE(V1)
{
  int value = param.asInt();

    if (value)
    {
      Serial.println("GoBack: " + (String)value);
      goBackwards();
    }
  
}
BLYNK_WRITE(V2)
{
  int value = param.asInt();

  if (value){
    Serial.println("Turn Left: " + (String)value);
    turnLeft();
    delay(200);
    motor_right(0);
motor_left(0); 
    
  }
}
BLYNK_WRITE(V3)
{
  int value = param.asInt();

  if (value){
    Serial.println("Turn Right: " + (String)value);
    turnRight();
    delay(200);
    motor_right(0);
    motor_left(0); 
  }
}

BLYNK_WRITE(V4)
{
  int value = param.asInt();

  if (value){
    Serial.println("Stop: " + (String)value);
    stop();
  }
}

BLYNK_WRITE(V5)
{
  int value = param.asInt();
  mode = value;
  
  
    if (mode==2)
    { sensor();
    }

  
}
BLYNK_WRITE(V9)
{
  int value = param.asInt();
  if (value==2)
  {
    mode = 3;
    Serial.print("switch sang mode 3");
    client.loop();
  }
}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
 Serial.print("] value:");
  String data;
  for (int i = 0; i < length; i++) {
    data += (char)payload[i];
  }
 
  int value = data.toInt();
  switch (value)  {
    case 0:
      stop();
      mode = 1;
      break;
    case 1:
      goStraightForwards();
      break;
    case 2:
      goBackwards();
      break;
    case 3:
      turnLeft();
      delay(200);
      motor_right(0);
      motor_left(0); 
      break;
    case 4:
      turnRight();
      delay(200);
      motor_right(0);
      motor_left(0); 
      break;  

    case 5:
      stop();
      
  }
  
  Serial.println((int) value);
}

void setup()
{

  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(sensor_right, INPUT);
  pinMode(sensor_left, INPUT);

  wifiConnect();
  client.setBufferSize(4096);
  client.subscribe(topic);
  client.setServer(server, 1883);
  client.setCallback(callback);
  Serial.println(mode);
  delay(1500);
  

}
//192.168.0.112
void loop()
{
  if (mode!=3)
  {
    Blynk.run();
    if (mode==2)
    {
      sensor();
    }
  }
  else{
    if (!client.connected()) 
    {
      mqttReConnect();
    }
  //client.publish(topic, new byte[0],0,true);
  //client.publish("test/topic", "1");
  client.loop();
  delay(100);
  }
  // Mode = 2 -> điều khiển bằng cảm biến
  // if(mode == 2){
  //   value_sensor_right = digitalRead(sensor_right);
  //   value_sensor_left = digitalRead(sensor_left);

  //   controlBySensor(value_sensor_right, value_sensor_left);
  // }
  // else if(mode ==1 ){
  //   Blynk.run();
  // }
}