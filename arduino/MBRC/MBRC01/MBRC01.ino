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

IPAddress local_IP(192, 168, 1, 12); // Static IP address192.168.100.164
IPAddress gateway(192, 168, 1, 1);    // Gateway IP address
IPAddress subnet(255, 255, 255, 0);     // subnet

char Machine_no[] = "MBRC01";

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
  ArduinoOTA.setHostname(Machine_no);

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

//String m108,m94,m76,m62,m60,m28,m26,m19,m6,m63,d330,d332,d334,d336,d338,d340,d342,d352,d344,d346,d348,d350,d32,d226,d616,d936;
//char d0[16],d1[16],d2[16],d3[16],d4[16],d5[16],d6[16],d7[16],d8[16],d9[16],d10[16],d11[16],d12[16],d13[16],d14[16],d15[16],d16[16],d17[16],d18[16],d19[16],d20[16],d21[16],d22[16],d23[16],d24[16],d25[16];
String rssi,D6000,D6002,D6004,D6100,D6102,D6104,D6010,D6020,D6030,D6040,D6050,D6012,D6022,D6032,D6042,D6052,D6080,D6082,D6084,D6086,D6088,D6090,D6310,D6300,D6312,D6302,D32,D6060,D6062,D6070,D6072,D616,D936;

char d_rssi[16],d1[16],d2[16],d3[16],d4[16],d5[16],d6[16],d7[16],d8[16],d9[16],d10[16],d11[16],d12[16],d13[16],d14[16],d15[16],d16[16],d17[16],d18[16],d19[16],d20[16],d21[16],d22[16],d23[16],d24[16],d25[16],d26[16],d27[16],d28[16],d29[16],d30[16],d31[16],d32[16],d33[16];

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
  Serial.print("Daily Total : ");
  Serial.println(node.getResponseBuffer(0));
  D6000 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(1, 1); 
  Serial.print("Daily OK : ");
  Serial.println(node.getResponseBuffer(0));
  D6002 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(2, 1); 
  Serial.print("Daily NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6004 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(3, 1); 
  Serial.print("Shift Total : ");
  Serial.println(node.getResponseBuffer(0));
  D6100 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(4, 1); 
  Serial.print("Shift OK : ");
  Serial.println(node.getResponseBuffer(0));
  D6102 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(5, 1); 
  Serial.print("Shift NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6104 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(6, 1); 
  Serial.print("C1 OK : ");
  Serial.println(node.getResponseBuffer(0));
  D6010 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(7, 1); 
  Serial.print("C2 OK : ");
  Serial.println(node.getResponseBuffer(0));
  D6020 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(8, 1); 
  Serial.print("C3 OK : ");
  Serial.println(node.getResponseBuffer(0));
  D6030 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(9, 1); 
  Serial.print("C4 OK : ");
  Serial.println(node.getResponseBuffer(0));
  D6040 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(10, 1); 
  Serial.print("C5 OK : ");
  Serial.println(node.getResponseBuffer(0));
  D6050 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(11, 1); 
  Serial.print("C1 NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6012 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(12, 1); 
  Serial.print("C2 NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6022 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(13, 1); 
  Serial.print("C3 NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6032 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(14, 1); 
  Serial.print("C4 NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6042 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(15, 1); 
  Serial.print("C5 NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6052 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  node.readHoldingRegisters(16, 1); 
  Serial.print("D2 RTNR NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6080 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(17, 1); 
  Serial.print("D1 RTNR NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6082 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(18, 1); 
  Serial.print("PRE PRESS NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6084 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(19, 1); 
  Serial.print("MAIN PRESS NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6086 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(20, 1); 
  Serial.print("PRESS CHECK NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6088 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(21, 1); 
  Serial.print("RTNR CAMERA NG : ");
  Serial.println(node.getResponseBuffer(0));
  D6090 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(22, 1); 
  Serial.print("BALL NG TOTAL DAILY : ");
  Serial.println(node.getResponseBuffer(0));
  D6310 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(23, 1); 
  Serial.print("BALL NG TOTAL SHIFT : ");
  Serial.println(node.getResponseBuffer(0));
  D6300 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(24, 1); 
  Serial.print("NG DATA TOTAL DAILY : ");
  Serial.println(node.getResponseBuffer(0));
  D6312 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(25, 1); 
  Serial.print("NG DATA TOTAL SHIFT : ");
  Serial.println(node.getResponseBuffer(0));
  D6302 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(26, 1); 
  Serial.print("CYCLE TIME : ");
  Serial.println(node.getResponseBuffer(0));
  D32 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(27, 1); 
  Serial.print("BALL CHECK CAMERA Q'TY : ");
  Serial.println(node.getResponseBuffer(0));
  D6060 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  delay(500);
  
  node.readHoldingRegisters(28, 1); 
  Serial.print("BALL CHECK CAMERA ANGLE : ");
  Serial.println(node.getResponseBuffer(0));
  D6062 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  delay(500);

  node.readHoldingRegisters(29, 1); 
  Serial.print("SEPARATE NG 1ST : ");
  Serial.println(node.getResponseBuffer(0));
  D6070 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  delay(500);

  node.readHoldingRegisters(30, 1); 
  Serial.print("SEPARATE NG 2ND : ");
  Serial.println(node.getResponseBuffer(0));
  D6072 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  delay(500);

  node.readHoldingRegisters(31, 1); 
  Serial.print("MATCHING UTL : ");
  Serial.println(node.getResponseBuffer(0));
  D616 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  delay(500);

  node.readHoldingRegisters(32, 1); 
  Serial.print("RETAINER UTL : ");
  Serial.println(node.getResponseBuffer(0));
  D936 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  delay(500);
  
  digitalWrite(led_run, HIGH);  
  
  D6000.toCharArray(d1, 16);client.publish("MBRC01/D6000", d1);
  D6002.toCharArray(d2, 16);client.publish("MBRC01/D6002", d2);
  D6004.toCharArray(d3, 16);client.publish("MBRC01/D6004", d3);
  D6100.toCharArray(d4, 16);client.publish("MBRC01/D6100", d4);
  D6102.toCharArray(d5, 16);client.publish("MBRC01/D6102", d5);
  D6104.toCharArray(d6, 16);client.publish("MBRC01/D6104", d6);
  D6010.toCharArray(d7, 16);client.publish("MBRC01/D6010", d7);
  D6020.toCharArray(d8, 16);client.publish("MBRC01/D6020", d8);
  D6030.toCharArray(d9, 16);client.publish("MBRC01/D6030", d9);
  D6040.toCharArray(d10, 16);client.publish("MBRC01/D6040", d10);
  D6050.toCharArray(d11, 16);client.publish("MBRC01/D6050", d11);
  D6012.toCharArray(d12, 16);client.publish("MBRC01/D6012", d12);
  D6022.toCharArray(d13, 16);client.publish("MBRC01/D6022", d13);
  D6032.toCharArray(d14, 16);client.publish("MBRC01/D6032", d14);
  D6042.toCharArray(d15, 16);client.publish("MBRC01/D6042", d15);
  D6052.toCharArray(d16, 16);client.publish("MBRC01/D6052", d16);
  D6080.toCharArray(d17, 16);client.publish("MBRC01/D6080", d17);
  D6082.toCharArray(d18, 16);client.publish("MBRC01/D6082", d18);
  D6084.toCharArray(d19, 16);client.publish("MBRC01/D6084", d19);
  D6086.toCharArray(d20, 16);client.publish("MBRC01/D6086", d20);
  D6088.toCharArray(d21, 16);client.publish("MBRC01/D6088", d21);
  D6090.toCharArray(d22, 16);client.publish("MBRC01/D6090", d22);
  D6310.toCharArray(d23, 16);client.publish("MBRC01/D6310", d23);
  D6300.toCharArray(d24, 16);client.publish("MBRC01/D6300", d24);
  D6312.toCharArray(d25, 16);client.publish("MBRC01/D6312", d25);
  D6302.toCharArray(d26, 16);client.publish("MBRC01/D6302", d26);
  D32.toCharArray(d27, 16);client.publish("MBRC01/D32", d27);
  D6060.toCharArray(d28, 16);client.publish("MBRC01/D6060", d28);
  D6062.toCharArray(d29, 16);client.publish("MBRC01/D6062", d29);
  D6070.toCharArray(d30, 16);client.publish("MBRC01/D6070", d30);
  D6072.toCharArray(d31, 16);client.publish("MBRC01/D6072", d31);
  D616.toCharArray(d32, 16);client.publish("MBRC01/D616", d32);
  D936.toCharArray(d33, 16);client.publish("MBRC01/D936", d33);
  rssi.toCharArray(d_rssi, 16);client.publish("MBRC01/rssi", d_rssi);
  
  Serial.println("\n---------------finish loop------------------\n\n");
  delay(5000);
}