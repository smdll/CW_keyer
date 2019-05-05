#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

const char *ssid = "CW_keyer";
ESP8266WebServer server(80);
uint8_t wpm = 20;
const uint8_t outpin = LED_BUILTIN;

void dit() {
  digitalWrite(outpin, LOW);
  delay(1200 / wpm);
  digitalWrite(outpin, HIGH);
  delay(1200 / wpm);
}

void dah() {
  digitalWrite(outpin, LOW);
  delay(3600 / wpm);
  digitalWrite(outpin, HIGH);
  delay(1200 / wpm);
}

void space() {
  delay(1200 / wpm);
}

void playLetter(char x) {
  switch (x) {
    case 'A':
      dit(); dah(); return;
    case 'B':
      dah(); dit(); dit(); dit(); return;
    case 'C':
      dah(); dit(); dah(); dit(); return;
    case 'D':
      dah(); dit(); dit(); return;
    case 'E':
      dit(); return;
    case 'F':
      dit(); dit(); dah(); dit(); return;
    case 'G':
      dah(); dah(); dit(); return;
    case 'H':
      dit(); dit(); dit(); dit(); return;
    case 'I':
      dit(); dit(); return;
    case 'J':
      dit(); dah(); dah(); dah(); return;
    case 'K':
      dah(); dit(); dah(); return;
    case 'L':
      dit(); dah(); dit(); dit(); return;
    case 'M':
      dah(); dah();
    case 'N':
      dah(); dit(); return;
    case 'O':
      dah(); dah(); dah(); return;
    case 'P':
      dit(); dah(); dah(); dit(); return;
    case 'Q':
      dah(); dah(); dit(); dah(); return;
    case 'R':
      dit(); dah(); dit(); return;
    case 'S':
      dit(); dit(); dit(); return;
    case 'T':
      dah(); return;
    case 'U':
      dit(); dit(); dah(); return;
    case 'V':
      dit(); dit(); dit(); dah(); return;
    case 'W':
      dit(); dah(); dah(); return;
    case 'X':
      dah(); dit(); dit(); dah(); return;
    case 'Y':
      dah(); dit(); dah(); dah(); return;
    case 'Z':
      dah(); dah(); dit(); dit(); return;
    case ' ':
      space(); space(); return;
    case '.':
      dit(); dah(); dit(); dah(); dit(); dah(); return;
    case '/':
      dah(); dit(); dit(); dah(); dit(); return;
    case '-':
      dah(); dit(); dit(); dit(); dit(); dah(); return;
    case '?':
      dit(); dah(); dit(); dit(); dah(); dit();  return;
    case ',':
      dah(); dah(); dit(); dit(); dah(); dah(); return;
    case '1':
      dit(); dah(); dah(); dah(); dah(); return;
    case '2':
      dit(); dit(); dah(); dah(); dah(); return;
    case '3':
      dit(); dit(); dit(); dah(); dah(); return;
    case '4':
      dit(); dit(); dit(); dit(); dah(); return;
    case '5':
      dit(); dit(); dit(); dit(); dit(); return;
    case '6':
      dah(); dit(); dit(); dit(); dit(); return;
    case '7':
      dah(); dah(); dit(); dit(); dit(); return;
    case '8':
      dah(); dah(); dah(); dit(); dit(); return;
    case '9':
      dah(); dah(); dah(); dah(); dit(); return;
    case '0':
      dah(); dah(); dah(); dah(); dah(); return;
  }
}

void handleFileRead(String path) {
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, "text/html");
    file.close();
  }
}

void onroot() {
  handleFileRead("/index.html");
}

void onsend() {
  String tosend;
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "tosend")
         tosend = server.arg(i);
      else if (server.argName(i) == "wpm")
         wpm = server.arg(i).toInt();
    }
  }
  server.send(200, "text/html", "<script language=\"javascript\">alert(\"Sending...\");window.location.href=\"/\";</script>");
  for (uint8_t i = 0; i < tosend.length(); i++) {
    char ch = tosend[i];
    if (isLowerCase(ch))
      ch = ch - 'a' + 'A';
    playLetter(ch);
    space();
  }
}

void setup() {
  pinMode(outpin, OUTPUT);
  digitalWrite(outpin, HIGH);//Turn off
  SPIFFS.begin();
  WiFi.softAP(ssid);
  server.on("/", onroot);
  server.on("/send", onsend);
  server.begin();
}

void loop() {
  server.handleClient();
}
