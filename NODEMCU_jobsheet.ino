
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "Realme1";
const char* password = "12345678";
const char* mqtt_server = " 192.168.43.50";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void reconnect() {

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
  
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
   
      delay(5000);
    }
  }
}

void setup() {
 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void publishSerialData(char *serialData)
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.publish("f", serialData);

  }

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

if (Serial.available() > 0) 
   {
     char a[101];
     memset(a,0, 101);
     Serial.readBytesUntil( '\n',a,100);
     publishSerialData(a);
   }

  }
