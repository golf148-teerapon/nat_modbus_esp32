#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ModbusMaster.h>

ModbusMaster node;

//#define led_connection 42
//#define led_run 41

#define rgb_red 41
#define rgb_green 42
#define rgb_blue 45

const char* ssid = "DX_Project";
const char* password = "natmms22";
//const char* ssid = "TP-Link_EF46";
//const char* password = "98148813";
const char* mqtt_server = "192.168.1.2";

IPAddress local_IP(192, 168, 1, 72); // Static IP address192.168.100.164
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
    delay(500);
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
  if (!client.connected()) 
  {
    led_red();
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
  pinMode(rgb_red, OUTPUT);
  pinMode(rgb_green, OUTPUT);
  pinMode(rgb_blue, OUTPUT);
  //pinMode ( led_connection , OUTPUT);
  //pinMode ( led_run, OUTPUT);
  Serial.println("Booting");
  setup_wifi(); 

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("AVS02");

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

String rssi,d310,d312,d318,d530,d262,d264,d266,d268,d270,d272,d274,d276,d278,d280,d282,d284,d286,d288,d290,d300 ;
char i_rssi[16],i0[16],i1[16],i2[16],i3[16],i4[16],i5[16],i6[16],i7[16],i8[16],i9[16],i10[16],i11[16],i12[16],i13[16],i14[16],i15[16],i16[16],i17[16],i18[16],i19[16] ;


void loop() 
{
  rssi = WiFi.RSSI();
  //Serial.print(WiFi.RSSI()); 
  ArduinoOTA.handle();
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
  led_green();
  //digitalWrite(led_connection, HIGH);
  Serial.println("\n---------------starting loop----------------");

  node.readHoldingRegisters(0, 1); 
  Serial.print("d310 : ");
  Serial.println(node.getResponseBuffer(0));
  d310 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(1, 1); 
  Serial.print("d312 : ");
  Serial.println(node.getResponseBuffer(0));
  d312 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(2, 1); 
  Serial.print("d318 : ");
  Serial.println(node.getResponseBuffer(0));
  d318 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  
  node.readHoldingRegisters(3, 1); 
  Serial.print("d530 : ");
  Serial.println(node.getResponseBuffer(0));
  d530 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(4, 1); 
  Serial.print("d262 : ");
  Serial.println(node.getResponseBuffer(0));
  d262 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(5, 1); 
  Serial.print("d264 : ");
  Serial.println(node.getResponseBuffer(0));
  d264 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(6, 1); 
  Serial.print("d266 : ");
  Serial.println(node.getResponseBuffer(0));
  d266 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(7, 1); 
  Serial.print("d268 : ");
  Serial.println(node.getResponseBuffer(0));
  d268 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(8, 1); 
  Serial.print("d270 : ");
  Serial.println(node.getResponseBuffer(0));
  d270 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(9, 1); 
  Serial.print("d272 : ");
  Serial.println(node.getResponseBuffer(0));
  d272 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(10, 1); 
  Serial.print("d274 : ");
  Serial.println(node.getResponseBuffer(0));
  d274 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(11, 1); 
  Serial.print("d276 : ");
  Serial.println(node.getResponseBuffer(0));
  d276 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(12, 1); 
  Serial.print("d278 : ");
  Serial.println(node.getResponseBuffer(0));
  d278 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(13, 1); 
  Serial.print("d280 : ");
  Serial.println(node.getResponseBuffer(0));
  d280 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(14, 1); 
  Serial.print("d282 : ");
  Serial.println(node.getResponseBuffer(0));
  d282 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

 node.readHoldingRegisters(15, 1); 
  Serial.print("d284 : "); 
  Serial.println(node.getResponseBuffer(0)); 
  d284 = node.getResponseBuffer(0); 
  node.clearResponseBuffer(); 

 node.readHoldingRegisters(16, 1); 
  Serial.print("d286 : "); 
  Serial.println(node.getResponseBuffer(0)); 
  d286 = node.getResponseBuffer(0); 
  node.clearResponseBuffer(); 

 node.readHoldingRegisters(17, 1); 
  Serial.print("d288 : "); 
  Serial.println(node.getResponseBuffer(0)); 
  d288 = node.getResponseBuffer(0); 
  node.clearResponseBuffer(); 

 node.readHoldingRegisters(18, 1); 
  Serial.print("d290 : "); 
  Serial.println(node.getResponseBuffer(0)); 
  d290 = node.getResponseBuffer(0); 
  node.clearResponseBuffer(); 

 node.readHoldingRegisters(19, 1); 
  Serial.print("d300 : "); 
  Serial.println(node.getResponseBuffer(0)); 
  d300 = node.getResponseBuffer(0); 
  node.clearResponseBuffer(); 

 
  delay(500);
  led_blue();
  
  rssi.toCharArray(i_rssi, 16);client.publish("AVS02/rssi", i_rssi);
  d310.toCharArray(i0, 16);client.publish("AVS02/D310", i0);
  d312.toCharArray(i1, 16);client.publish("AVS02/D312", i1);
  d318.toCharArray(i2, 16);client.publish("AVS02/D318", i2);
  d530.toCharArray(i3, 16);client.publish("AVS02/D530", i3);
  d262.toCharArray(i4, 16);client.publish("AVS02/D262", i4);
  d264.toCharArray(i5, 16);client.publish("AVS02/D264", i5);
  d266.toCharArray(i6, 16);client.publish("AVS02/D266", i6);
  d268.toCharArray(i7, 16);client.publish("AVS02/D268", i7);
  d270.toCharArray(i8, 16);client.publish("AVS02/D270", i8);
  d272.toCharArray(i9, 16);client.publish("AVS02/D272", i9);
  d274.toCharArray(i10, 16);client.publish("AVS02/D274", i10);
  d276.toCharArray(i11, 16);client.publish("AVS02/D276", i11);
  d278.toCharArray(i12, 16);client.publish("AVS02/D278", i12);
  d280.toCharArray(i13, 16);client.publish("AVS02/D280", i13);
  d282.toCharArray(i14, 16);client.publish("AVS02/D282", i14);
  d284.toCharArray(i15, 16);client.publish("AVS02/D284", i15);
  d286.toCharArray(i16, 16);client.publish("AVS02/D286", i16);
  d288.toCharArray(i17, 16);client.publish("AVS02/D288", i17);
  d290.toCharArray(i18, 16);client.publish("AVS02/D290", i18);
  d300.toCharArray(i19, 16);client.publish("AVS02/D300", i19);
  Serial.println("\n---------------finish loop------------------\n\n");
  delay(5000);
}

void led_red()
{
  digitalWrite(rgb_red, LOW); digitalWrite(rgb_green, HIGH); digitalWrite(rgb_blue, HIGH);  
}

void led_green()
{
  digitalWrite(rgb_red, HIGH); digitalWrite(rgb_green, LOW); digitalWrite(rgb_blue, HIGH);  
}

void led_blue()
{
  digitalWrite(rgb_red, HIGH); digitalWrite(rgb_green, HIGH); digitalWrite(rgb_blue, LOW);  
}

void rgb_off()
{
  digitalWrite(rgb_red, HIGH); digitalWrite(rgb_green, HIGH); digitalWrite(rgb_blue, HIGH);
}