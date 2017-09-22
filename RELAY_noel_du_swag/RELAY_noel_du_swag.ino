#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Trieu4 2.4GhHz";
const char* password = "Theo1990";

ESP8266WebServer server(80);

const int led = 13;
const int relay = 2;
const int relay1 = 0;
//const int relay2 = 4;
//const int relay3 = 3;
//const int relay4 = 4;
//const int relay5 = 5;
//const int relay6 = 6;
//const int relay7 = 7;


void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(relay, OUTPUT);
  pinMode(relay1, OUTPUT);
  /*pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  */
  //pinMode(relay7, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  digitalWrite(relay, HIGH);
  digitalWrite(relay1, HIGH);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/fire", [](){
    server.send(200, "text/plain", "this works as well");
    Serial.write("ok");
    digitalWrite(relay, LOW);
    delay(3000);
    Serial.write("boom les bitchez");
    digitalWrite(relay, HIGH);
  });

    server.on("/fire1", [](){
    server.send(200, "text/plain", "this works as well");
    digitalWrite(relay1, LOW);
    Serial.write("ok");
    delay(3000);
    Serial.write("boom les bitchez");
    digitalWrite(relay1, HIGH);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
