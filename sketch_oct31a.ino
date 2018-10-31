#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "hope";
const char* password = "hope87210";
WebServer server(80);

const int led = 13;

void handleRoot() {
  char webSite[1000];
  snprintf(webSite,1000,"<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>Diming LED</title> <script src=\"https://code.jquery.com/jquery-3.3.1.js\"></script> <script>$(document).ready(function(){$(\"[type=range]\").change(function(){var ledvalue=$(\"#led\").val(); $(\".ledvalue\").text(ledvalue);});}); </script> <style>body{background-color: lightgoldenrodyellow;}.container{display: flex; flex-direction: column; align-items: center;}.btn{padding: 0.3em 0.5em; font-size: 1.5em;}</style></head><body> <div class=\"container\"> <h1>LED Diming</h1> <h3>LED Light=<span class='ledvalue'>0</span></h3> <form action=\"ledDiming\" method=\"get\" id=\"form1\"> <input type=\"range\" name=\"ledval\" id=\"led\" min=\"0\" max=\"255\"> </form> <br><button type=\"submit\" form=\"form1\" value=\"Submit\" class=\"btn\">Submit</button> </div></body></html>");
  server.send(200, "text/html", webSite);
}

void ledDiming() {
  char webSite[1000];
  int led_Diming=server.arg("ledval").toInt();
  ledcWrite(0,led_Diming);
  server.send(200, "text/html", webSite);
}
void handleNotFound() {
  digitalWrite(led, 1);
  server.send(404, "text/html", "Page not founded...");
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  ledcSetup(0,500,8);
  ledcAttachPin (13,0);

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

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/ledDiming",ledDiming);
  server.onNotFound(handleNotFound);
  server.begin();
  server.on("/inline", []() {
    server.send(200, "text/html", "this works as well");
  });
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
