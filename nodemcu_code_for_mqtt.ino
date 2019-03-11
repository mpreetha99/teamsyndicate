#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Realme1";
const char* password = "12345678";
const char* mqtt_server = "iot.eclipse.org";

#define mqtt_port 1883
#define MQTT_USER "Rahul"
#define MQTT_PASSWORD "rahul"
#define MQTT "/rahul/SmartDustbin/"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup_wifi() 
{
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() 
{
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "NodeMCUClient-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) 
    {
      Serial.println("connected");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(500);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  reconnect();
}

void publishSerialData(char *serialData)
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.publish(MQTT, serialData);
  Serial.print(serialData);
  }

void loop() 
{
   client.loop();
   if (Serial.available() > 0) 
   {
     char a[101];
     memset(a,0, 101);
     Serial.readBytesUntil( '\n',a,100);
     publishSerialData(a);
   }
}
