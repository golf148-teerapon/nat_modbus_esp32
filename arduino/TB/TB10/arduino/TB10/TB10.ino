//TB10 
//change ssid,password,mqtt_server,local_IP,setHostname,String rssi,char i_rssi,client.publish

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

IPAddress local_IP(192, 168, 1, 40); // Static IP address192.168.100.164
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
  ArduinoOTA.setHostname("TB10");

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

String rssi,m108,m94,m76,m62,m60,m28,m26,m19,m6,m63,d330,d332,d334,d336,d338,d340,d342,d352,d344,d346,d348,d350,d32,d226,d354,d286,d242,d241,d240,d245,d244,d243,d100,d102,d104,d106,d108,d110,d112,d114,d116,d118,d120,d122,d124,d126,d128,m34,m36;
char i_rssi[16],i0[16],i1[16],i2[16],i3[16],i4[16],i5[16],i6[16],i7[16],i8[16],i9[16],i10[16],i11[16],i12[16],i13[16],i14[16],i15[16],i16[16],i17[16],i18[16],i19[16],i20[16],i21[16],i22[16],i23[16],i24[16],i25[16],i26[16],i27[16],i28[16],i29[16],i30[16],i31[16],i32[16],i33[16],i34[16],i35[16],i36[16],i37[16],i38[16],i39[16],i40[16],i41[16],i42[16],i43[16],i44[16],i45[16],i46[16],i47[16],i48[16],i49[16];

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

  node.readHoldingRegisters(33, 1); 
  Serial.print("d100 : ");
  Serial.println(node.getResponseBuffer(0));
  d100 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(34, 1); 
  Serial.print("d102 : ");
  Serial.println(node.getResponseBuffer(0));
  d102 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(35, 1); 
  Serial.print("d104 : ");
  Serial.println(node.getResponseBuffer(0));
  d104 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(36, 1); 
  Serial.print("d106 : ");
  Serial.println(node.getResponseBuffer(0));
  d106 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(37, 1); 
  Serial.print("d108 : ");
  Serial.println(node.getResponseBuffer(0));
  d108 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(38, 1); 
  Serial.print("d110 : ");
  Serial.println(node.getResponseBuffer(0));
  d110 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(39, 1); 
  Serial.print("d112 : ");
  Serial.println(node.getResponseBuffer(0));
  d112 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(40, 1); 
  Serial.print("d114 : ");
  Serial.println(node.getResponseBuffer(0));
  d114 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(41, 1); 
  Serial.print("d116 : ");
  Serial.println(node.getResponseBuffer(0));
  d116 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(42, 1); 
  Serial.print("d118 : ");
  Serial.println(node.getResponseBuffer(0));
  d118 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(42, 1); 
  Serial.print("d120 : ");
  Serial.println(node.getResponseBuffer(0));
  d120 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(44, 1); 
  Serial.print("d122 : ");
  Serial.println(node.getResponseBuffer(0));
  d122 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(45, 1); 
  Serial.print("d124 : ");
  Serial.println(node.getResponseBuffer(0));
  d124 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(46, 1); 
  Serial.print("d126 : ");
  Serial.println(node.getResponseBuffer(0));
  d126 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(47, 1); 
  Serial.print("d128 : ");
  Serial.println(node.getResponseBuffer(0));
  d128 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(48, 1); 
  Serial.print("d34 : ");
  Serial.println(node.getResponseBuffer(0));
  m34 = node.getResponseBuffer(0);
  node.clearResponseBuffer();

  node.readHoldingRegisters(49, 1); 
  Serial.print("m36 : ");
  Serial.println(node.getResponseBuffer(0));
  m36 = node.getResponseBuffer(0);
  node.clearResponseBuffer();
  
  delay(500);
  led_blue();
  rssi.toCharArray(i_rssi, 16);client.publish("TB10/rssi", i_rssi);
  m108.toCharArray(i0, 16);client.publish("TB10/M108", i0);
  m94.toCharArray(i1, 16);client.publish("TB10/M94", i1);
  m76.toCharArray(i2, 16);client.publish("TB10/M76", i2);
  m62.toCharArray(i3, 16);client.publish("TB10/M62", i3);
  m60.toCharArray(i4, 16);client.publish("TB10/M60", i4);
  m28.toCharArray(i5, 16);client.publish("TB10/M28", i5);
  m26.toCharArray(i6, 16);client.publish("TB10/M26", i6);
  m19.toCharArray(i7, 16);client.publish("TB10/M19", i7);
  m6.toCharArray(i8, 16);client.publish("TB10/M6", i8);
  m63.toCharArray(i9, 16);client.publish("TB10/M63", i9);
  d330.toCharArray(i10, 16);client.publish("TB10/D330", i10);
  d332.toCharArray(i11, 16);client.publish("TB10/D332", i11);
  d334.toCharArray(i12, 16);client.publish("TB10/D334", i12);
  d336.toCharArray(i13, 16);client.publish("TB10/D336", i13);
  d338.toCharArray(i14, 16);client.publish("TB10/D338", i14);
  d340.toCharArray(i15, 16);client.publish("TB10/D340", i15);
  d342.toCharArray(i16, 16);client.publish("TB10/D342", i16);
  d352.toCharArray(i17, 16);client.publish("TB10/D352", i17);
  d344.toCharArray(i18, 16);client.publish("TB10/D344", i18);
  d346.toCharArray(i19, 16);client.publish("TB10/D346", i19);
  d348.toCharArray(i20, 16);client.publish("TB10/D348", i20);
  d350.toCharArray(i21, 16);client.publish("TB10/D350", i21);
  d32.toCharArray(i22, 16);client.publish("TB10/D32", i22);
  d226.toCharArray(i23, 16);client.publish("TB10/D226", i23);
  d354.toCharArray(i24, 16);client.publish("TB10/D354", i24);
  d286.toCharArray(i25, 16);client.publish("TB10/D286", i25);
  d242.toCharArray(i26, 16);client.publish("TB10/D242", i26);
  d241.toCharArray(i27, 16);client.publish("TB10/D241", i27);
  d240.toCharArray(i28, 16);client.publish("TB10/D240", i28);
  d245.toCharArray(i29, 16);client.publish("TB10/D245", i29);
  d244.toCharArray(i30, 16);client.publish("TB10/D244", i30);
  d243.toCharArray(i31, 16);client.publish("TB10/D243", i31);
  d100.toCharArray(i32, 16);client.publish("TB10/D100", i32);
  d102.toCharArray(i33, 16);client.publish("TB10/D102", i33);
  d104.toCharArray(i34, 16);client.publish("TB10/D104", i34);
  d106.toCharArray(i35, 16);client.publish("TB10/D106", i35);
  d108.toCharArray(i36, 16);client.publish("TB10/D108", i36);
  d110.toCharArray(i37, 16);client.publish("TB10/D110", i37);
  d112.toCharArray(i38, 16);client.publish("TB10/D112", i38);
  d114.toCharArray(i39, 16);client.publish("TB10/D114", i39);
  d116.toCharArray(i40, 16);client.publish("TB10/D116", i40);
  d118.toCharArray(i41, 16);client.publish("TB10/D118", i41);
  d120.toCharArray(i42, 16);client.publish("TB10/D120", i42);
  d122.toCharArray(i43, 16);client.publish("TB10/D122", i43);
  d124.toCharArray(i44, 16);client.publish("TB10/D124", i44);
  d126.toCharArray(i45, 16);client.publish("TB10/D126", i45);
  d128.toCharArray(i46, 16);client.publish("TB10/D128", i46);
  m34.toCharArray(i47, 16);client.publish("TB10/M34", i47);
  m36.toCharArray(i48, 16);client.publish("TB10/M36", i48);
     
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
