#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

#define STASSID "ENTER_SSID_HERE"
#define STAPSK  "ENTER_PSWD_HERE"
const char* www_username = "ENTER_FIRST_COMMAND_HERE";
const char* www_username2 = "ENTER_SECOND_COMMAND_HERE"; //Add more commands if desired
const char* www_password = "ENTER_PSWD_HERE";


const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Setup Start");
  pinMode(2,OUTPUT); //connected to a relay switch
  //Wifi Settings
  WiFi.mode(WIFI_STA);
  IPAddress ip(192, 168, 0, 200);   // St a static IP for NGROK.
  IPAddress gateway(192,168,0,1);   // Router IP, may be 192.168.1.1 for other people
  IPAddress subnet(255,255,255,0);  // Default subnet mask with 0 subnet bits
  WiFi.config(ip,gateway,subnet);   // Set WiFi config with new settings
  WiFi.begin(ssid, password);  
  //Connect to wifi
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  ArduinoOTA.begin();

  //Start webserver
  server.on("/", []() {
    if (server.authenticate(www_username, www_password)){ //if first creds correct
      server.send(200, "text/plain", "PC Power Pressed"); //if creds correct, change site html
      digitalWrite(2,HIGH); //press button for 0.5 seconds
      delay(500);
      digitalWrite(2,LOW);
    }else if(server.authenticate(www_username2, www_password)){ //if second creds correct
      server.send(200, "text/plain", "PC Power Pressed"); //if creds correct, change site html
      digitalWrite(2,HIGH); //press button for 5.5 seconds
      delay(5500);
      digitalWrite(2,LOW);
    }else{ //Otherwise if all creditentials are incorrect
        delay(5000); //delay response to make brute force attacks less practical
        return server.requestAuthentication();
    }
  });
  server.begin();
  Serial.print("Server running on http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
