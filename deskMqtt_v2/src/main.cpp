#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <PwmTransition.h>
const char* ssid = "Lisby";
const char* password = "lisby12345";
const char* mqtt_server = "10.0.1.14";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient desktopClientESP;
PubSubClient client(desktopClientESP);

PwmTransition pwmT;

//Outputs
const int lamp  = 13;

//Inputs
const int butIn = 14;

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;
int tempRead = 0;  //For storing temperature analog value.
float computedTemp = 0; //Stores de real temperature value.
int messageBuffer_int = 0;
int lastValue = 0;
String messageTemp;
int prevMessageTemp;
bool prevStat = false;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  messageTemp = "";

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if(topic=="room/desktop"){
    Serial.print("Changing Room desktop to: ");
    Serial.println(messageTemp);
    int messageBuffer = 0;
    if(messageTemp == "true"){    //writes buffer if the input is ON/OFF
      messageBuffer = 100;
      Serial.print("On");
    } else if(messageTemp == "false"){
      messageBuffer = 0;
      Serial.print("Off");
    }else{
      messageBuffer = messageTemp.toInt();
    }
  messageBuffer = map(messageBuffer,0,100,0,1000);
  pwmT.fadeTransition(lamp, messageBuffer, prevMessageTemp, 3);
  prevMessageTemp = messageBuffer;
  Serial.println();
  }
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void reconnect(){
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("Desktop_Client")) {
      Serial.println("connected");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("room/desktop");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void startOTA(){
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(lamp, OUTPUT);
  pinMode(butIn,INPUT_PULLUP);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  startOTA();
}

void loop() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("DesktopClient");

  if((digitalRead(butIn)==LOW)&&prevStat==false){
    pwmT.fadeTransition(lamp, 1000, prevMessageTemp, 3);
    prevMessageTemp = 1000;
    prevStat = true;
    delay(100);
  }else if((digitalRead(butIn)==LOW)&&prevStat==true){
    pwmT.fadeTransition(lamp, 0, prevMessageTemp, 3);
    prevMessageTemp = 0;
    prevStat = false;
    delay(100);
  }
}
