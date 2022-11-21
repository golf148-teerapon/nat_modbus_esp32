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

IPAddress local_IP(192, 168, 1, 5); // Static IP address192.168.100.164
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
  ArduinoOTA.setHostname("first_setup");

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

String rssi,m108,m94,m76,m62,m60,m28,m26,m19,m6,m63,d330,d332,d334,d336,d338,d340,d342,d352,d344,d346,d348,d350,d32,d226,d354,d286,d242,d241,d240,d245,d244,d243;
char d_rssi[16],d0[16],d1[16],d2[16],d3[16],d4[16],d5[16],d6[16],d7[16],d8[16],d9[16],d10[16],d11[16],d12[16],d13[16],d14[16],d15[16],d16[16],d17[16],d18[16],d19[16],d20[16],d21[16],d22[16],d23[16],d24[16],d25[16],d26[16],d27[16],d28[16],d29[16],d30[16],d31[16];


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
  Serial.print("M108 : ");
  Serial.println(node.getResponseBuffer(0));
  m108 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(1, 1); 
  Serial.print("M94 : ");
  Serial.println(node.getResponseBuffer(0));
  m94 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(2, 1); 
  Serial.print("M76 : ");
  Serial.println(node.getResponseBuffer(0));
  m76 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(3, 1); 
  Serial.print("M62 : ");
  Serial.println(node.getResponseBuffer(0));
  m62 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(4, 1); 
  Serial.print("M60 : ");
  Serial.println(node.getResponseBuffer(0));
  m60 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(5, 1); 
  Serial.print("M28 : ");
  Serial.println(node.getResponseBuffer(0));
  m28 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(6, 1); 
  Serial.print("M26 : ");
  Serial.println(node.getResponseBuffer(0));
  m26 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(7, 1); 
  Serial.print("M19 : ");
  Serial.println(node.getResponseBuffer(0));
  m19 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(8, 1); 
  Serial.print("M6 : ");
  Serial.println(node.getResponseBuffer(0));
  m6 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(9, 1); 
  Serial.print("M63 : ");
  Serial.println(node.getResponseBuffer(0));
  m63 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(10, 1); 
  Serial.print("D330 : ");
  Serial.println(node.getResponseBuffer(0));
  d330 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(11, 1); 
  Serial.print("D332 : ");
  Serial.println(node.getResponseBuffer(0));
  d332 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(12, 1); 
  Serial.print("D334 : ");
  Serial.println(node.getResponseBuffer(0));
  d334 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(13, 1); 
  Serial.print("D336 : ");
  Serial.println(node.getResponseBuffer(0));
  d336 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(14, 1); 
  Serial.print("D338 : ");
  Serial.println(node.getResponseBuffer(0));
  d338 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(15, 1); 
  Serial.print("D340 : ");
  Serial.println(node.getResponseBuffer(0));
  d340 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(16, 1); 
  Serial.print("D342 : ");
  Serial.println(node.getResponseBuffer(0));
  d342 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(17, 1); 
  Serial.print("D352 : ");
  Serial.println(node.getResponseBuffer(0));
  d352 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(18, 1); 
  Serial.print("D344 : ");
  Serial.println(node.getResponseBuffer(0));
  d344 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(19, 1); 
  Serial.print("D346 : ");
  Serial.println(node.getResponseBuffer(0));
  d346 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(20, 1); 
  Serial.print("D348 : ");
  Serial.println(node.getResponseBuffer(0));
  d348 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(21, 1); 
  Serial.print("D350 : ");
  Serial.println(node.getResponseBuffer(0));
  d350 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(22, 1); 
  Serial.print("D32 : ");
  Serial.println(node.getResponseBuffer(0));
  d32 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(23, 1); 
  Serial.print("D226 : ");
  Serial.println(node.getResponseBuffer(0));
  d226 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(24, 1); 
  Serial.print("d354 : ");
  Serial.println(node.getResponseBuffer(0));
  d354 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(25, 1); 
  Serial.print("d286 : ");
  Serial.println(node.getResponseBuffer(0));
  d286 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(26, 1); 
  Serial.print("d242 : ");
  Serial.println(node.getResponseBuffer(0));
  d242 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(27, 1); 
  Serial.print("d241 : ");
  Serial.println(node.getResponseBuffer(0));
  d241 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(28, 1); 
  Serial.print("d240 : ");
  Serial.println(node.getResponseBuffer(0));
  d240 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(29, 1); 
  Serial.print("d245 : ");
  Serial.println(node.getResponseBuffer(0));
  d245 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(30, 1); 
  Serial.print("d244 : ");
  Serial.println(node.getResponseBuffer(0));
  d244 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(31, 1); 
  Serial.print("d243 : ");
  Serial.println(node.getResponseBuffer(0));
  d243 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  delay(500);
  led_blue();
  rssi.toCharArray(d_rssi, 16);client.publish("TN01/rssi", d_rssi);
  m108.toCharArray(d0, 16);client.publish("TN01/M108", d0);
  m94.toCharArray(d1, 16);client.publish("TN01/M94", d1);
  m76.toCharArray(d2, 16);client.publish("TN01/M76", d2);
  m62.toCharArray(d3, 16);client.publish("TN01/M62", d3);
  m60.toCharArray(d4, 16);client.publish("TN01/M60", d4);
  m28.toCharArray(d5, 16);client.publish("TN01/M28", d5);
  m26.toCharArray(d6, 16);client.publish("TN01/M26", d6);
  m19.toCharArray(d7, 16);client.publish("TN01/M19", d7);
  m6.toCharArray(d8, 16);client.publish("TN01/M6", d8);
  m63.toCharArray(d9, 16);client.publish("TN01/M63", d9);
  d330.toCharArray(d10, 16);client.publish("TN01/D330", d10);
  d332.toCharArray(d11, 16);client.publish("TN01/D332", d11);
  d334.toCharArray(d12, 16);client.publish("TN01/D334", d12);
  d336.toCharArray(d13, 16);client.publish("TN01/D336", d13);
  d338.toCharArray(d14, 16);client.publish("TN01/D338", d14);
  d340.toCharArray(d15, 16);client.publish("TN01/D340", d15);
  d342.toCharArray(d16, 16);client.publish("TN01/D342", d16);
  d352.toCharArray(d17, 16);client.publish("TN01/D352", d17);
  d344.toCharArray(d18, 16);client.publish("TN01/D344", d18);
  d346.toCharArray(d19, 16);client.publish("TN01/D346", d19);
  d348.toCharArray(d20, 16);client.publish("TN01/D348", d20);
  d350.toCharArray(d21, 16);client.publish("TN01/D350", d21);
  d32.toCharArray(d22, 16);client.publish("TN01/D32", d22);
  d226.toCharArray(d23, 16);client.publish("TN01/D226", d23);
  d354.toCharArray(d24, 16);client.publish("TN01/D354", d24);
  d286.toCharArray(d25, 16);client.publish("TN01/D286", d25);
  d242.toCharArray(d26, 16);client.publish("TN01/D242", d26);
  d241.toCharArray(d27, 16);client.publish("TN01/D241", d27);
  d240.toCharArray(d28, 16);client.publish("TN01/D240", d28);
  d245.toCharArray(d29, 16);client.publish("TN01/D245", d29);
  d244.toCharArray(d30, 16);client.publish("TN01/D244", d30);
  d243.toCharArray(d31, 16);client.publish("TN01/D243", d31);
  
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
