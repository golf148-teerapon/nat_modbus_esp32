//IRH03
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ModbusMaster.h>

ModbusMaster node;

#define led_connection 41
#define led_run 42


const char* ssid = "DX_Project";
const char* password = "natmms22";
//const char* ssid = "TP-Link_EF46";
//const char* password = "98148813";
const char* mqtt_server = "192.168.1.2";

IPAddress local_IP(192, 168, 1, 83); // Static IP address192.168.100.164
IPAddress gateway(192, 168, 1, 1);    // Gateway IP address
IPAddress subnet(255, 255, 255, 0);     // subnet

WiFiClient espClient;
PubSubClient client(espClient);

/*void led()
{
  digitalWrite(led_connection , HIGH);
  delay(100);
  digitalWrite(led_connection , LOW);
  delay(100);
}*/

int count_connection;

void setup_wifi() 
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.config(local_IP, gateway, subnet);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    count_connection++;
    digitalWrite(led_connection, HIGH);delay(100);digitalWrite(led_connection, LOW);delay(100);  
    Serial.print(".");
    if(count_connection>20)
    {
      ESP.restart();
    }
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    //digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

int timeout ;

void reconnect() 
{
  // Loop until we're reconnected
  if(!client.connected()) 
  {
    digitalWrite(led_connection, HIGH);  
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      //delay(5000);
      digitalWrite(led_connection, HIGH);delay(300);digitalWrite(led_connection, LOW);delay(300);    
      timeout++;
      if(timeout >= 10)
      {
        ESP.restart();
      }
    }
  }
}

void setup() 
{
  Serial.begin(9600);
  Serial1.begin(9600);
  node.begin(1,Serial1);
  pinMode ( led_connection , OUTPUT);
  pinMode ( led_run, OUTPUT);
  Serial.println("Booting");
  setup_wifi(); 

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("IRH03");

  // No authentication by default
  ArduinoOTA.setPassword("1234");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
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
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

//String m108,m94,m76,m62,m60,m28,m26,m19,m6,m63,d330,d332,d334,d336,d338,d340,d342,d352,d344,d346,d348,d350,d32,d226;
//char d0[16],d1[16],d2[16],d3[16],d4[16],d5[16],d6[16],d7[16],d8[16],d9[16],d10[16],d11[16],d12[16],d13[16],d14[16],d15[16],d16[16],d17[16],d18[16],d19[16],d20[16],d21[16],d22[16],d23[16];
String rssi,zr2,zr6,d5210,d5230,d5250,zr532,zr552,zr572,zr632,zr652,zr672,zr732,zr752,zr772;
char d_rssi[16],d0[16],d1[16],d2[16],d3[16],d4[16],d5[16],d6[16],d7[16],d8[16],d9[16],d10[16],d11[16],d12[16],d13[16];


void loop() 
{
  rssi = WiFi.RSSI();
  ArduinoOTA.handle();
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
  digitalWrite(led_connection, HIGH);  
  digitalWrite(led_run, LOW);  
  //digitalWrite(led_connection, HIGH);
  Serial.println("\n---------------starting loop----------------");
  
  node.readHoldingRegisters(0, 1); 
  Serial.print("zr2 : ");
  Serial.println(node.getResponseBuffer(0));
  zr2 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(1, 1); 
  Serial.print("zr6 : ");
  Serial.println(node.getResponseBuffer(0));
  zr6 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(2, 1); 
  Serial.print("d5210 : ");
  Serial.println(node.getResponseBuffer(0));
  d5210 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(3, 1); 
  Serial.print("d5230 : ");
  Serial.println(node.getResponseBuffer(0));
  d5230 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(4, 1); 
  Serial.print("d5250 : ");
  Serial.println(node.getResponseBuffer(0));
  d5250 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(5, 1); 
  Serial.print("zr532 : ");
  Serial.println(node.getResponseBuffer(0));
  zr532 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(6, 1); 
  Serial.print("zr552 : ");
  Serial.println(node.getResponseBuffer(0));
  zr552 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(7, 1); 
  Serial.print("zr572 : ");
  Serial.println(node.getResponseBuffer(0));
  zr572 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(8, 1); 
  Serial.print("zr632 : ");
  Serial.println(node.getResponseBuffer(0));
  zr632 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(9, 1); 
  Serial.print("zr652 : ");
  Serial.println(node.getResponseBuffer(0));
  zr652 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(10, 1); 
  Serial.print("zr672 : ");
  Serial.println(node.getResponseBuffer(0));
  zr672 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(11, 1); 
  Serial.print("zr732 : ");
  Serial.println(node.getResponseBuffer(0));
  zr732 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(12, 1); 
  Serial.print("zr752 : ");
  Serial.println(node.getResponseBuffer(0));
  zr752 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(13, 1); 
  Serial.print("zr772 : ");
  Serial.println(node.getResponseBuffer(0));
  zr772 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  delay(500);
  digitalWrite(led_run, HIGH);  
  
  zr2.toCharArray(d0, 16);client.publish("IRH03/ZR2", d0);
  zr6.toCharArray(d1, 16);client.publish("IRH03/ZR6", d1);
  d5210.toCharArray(d2, 16);client.publish("IRH03/D5210", d2);
  d5230.toCharArray(d3, 16);client.publish("IRH03/D5230", d3);
  d5250.toCharArray(d4, 16);client.publish("IRH03/D5250", d4);
  zr532.toCharArray(d5, 16);client.publish("IRH03/ZR532", d5);
  zr552.toCharArray(d6, 16);client.publish("IRH03/ZR552", d6);
  zr572.toCharArray(d7, 16);client.publish("IRH03/ZR572", d7);
  zr632.toCharArray(d8, 16);client.publish("IRH03/ZR632", d8);
  zr652.toCharArray(d9, 16);client.publish("IRH03/ZR652", d9);
  zr672.toCharArray(d10, 16);client.publish("IRH03/ZR672", d10);
  zr732.toCharArray(d11, 16);client.publish("IRH03/ZR732", d11);
  zr752.toCharArray(d12, 16);client.publish("IRH03/ZR752", d12);
  zr772.toCharArray(d13, 16);client.publish("IRH03/ZR772", d13);
  rssi.toCharArray(d_rssi, 16);client.publish("IRH03/rssi", d_rssi);
  
  Serial.println("\n---------------finish loop------------------\n\n");
  delay(5000);
}