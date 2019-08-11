#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "PSLab_ESP";

ESP8266WebServer server(80);

//Simple http server to handle different requests on endpoints

//Method to handle root endpoint
void handleRoot() {
  server.send(200, "text/plain", "hello PSLab");
}

//Method to handle 404 error
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  //Create wifi accesspoint
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();

  //Declare endpoints here
  server.on("/", handleRoot);

  //endpoint to get vesion of PSLab
  server.on("/version", []() {
    server.send(200, "text/plain", "This will return PSLab version");
  });

  //end point for 404 error
  server.onNotFound(handleNotFound);

  //initiate the server
  server.begin();
}

void loop(void) {
  server.handleClient();
}
