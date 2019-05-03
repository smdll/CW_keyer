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

void di() {
  dit();
}

void playLetter(char x) {
  switch (x) {
    case 'A':
      di(); dah(); return;
    case 'B':
      dah(); di(); di(); dit(); return;
    case 'C':
      dah(); di(); dah(); dit(); return;
    case 'D':
      dah(); di(); di(); return;
    case 'E':
      di(); return;
    case 'F':
      di(); di(); dah(); dit(); return;
    case 'G':
      dah(); dah(); dit(); return;
    case 'H':
      di(); di(); di(); dit(); return;
    case 'I':
      di(); dit(); return;
    case 'J':
      di(); dah(); dah(); dah(); return;
    case 'K':
      dah(); di(); dah(); return;
    case 'L':
      di(); dah(); di(); dit(); return;
    case 'M':
      dah(); dah();
    case 'N':
      dah(); dit(); return;
    case 'O':
      dah(); dah(); dah(); return;
    case 'P':
      di(); dah(); dah(); dit(); return;
    case 'Q':
      dah(); dah(); di(); dah(); return;
    case 'R':
      di(); dah(); dit(); return;
    case 'S':
      di(); di(); di(); return;
    case 'T':
      dah(); return;
    case 'U':
      di(); di(); dah(); return;
    case 'V':
      di(); di(); di(); dah(); return;
    case 'W':
      di(); dah(); dah(); return;
    case 'X':
      dah(); di(); di(); dah(); return;
    case 'Y':
      dah(); di(); dah(); dah(); return;
    case 'Z':
      dah(); dah(); di(); dit(); return;
    case ' ':
      space(); space(); return;
    case '.':
      di(); dah(); di(); dah(); di(); dah(); return;
    case '/':
      dah(); di(); di(); dah(); di(); return;
    case '-':
      dah(); di(); di(); di(); di(); dah(); return;
    case '?':
      di(); dah(); di(); di(); dah(); di();  return;
    case ',':
      dah(); dah(); di(); di(); dah(); dah(); return;
    case '1':
      di(); dah(); dah(); dah(); dah(); return;
    case '2':
      di(); di(); dah(); dah(); dah(); return;
    case '3':
      di(); di(); di(); dah(); dah(); return;
    case '4':
      di(); di(); di(); di(); dah(); return;
    case '5':
      di(); di(); di(); di(); di(); return;
    case '6':
      dah(); di(); di(); di(); di(); return;
    case '7':
      dah(); dah(); di(); di(); di(); return;
    case '8':
      dah(); dah(); dah(); di(); di(); return;
    case '9':
      dah(); dah(); dah(); dah(); di(); return;
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
  for (uint8_t i = 0; i < tosend.length(); i++) {
    char ch = tosend[i];
    if (isLowerCase(ch))
      ch = ch - 'a' + 'A';
    playLetter(ch);
    space();
  }
  server.send(200, "application/javascript", "alert(\"Sent\");window.location.href=\"/\";");
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
