/*
  Based off Web Server Example code by David A. Mellis,  Tom Igoe and  Arturo Guadalupi
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Arduino_JSON.h>

byte mac[] = {
  0xA8, 0x61, 0x0A, 0xAE, 0xAB, 0x54
};
IPAddress ip(192, 168, 1, 42);
byte subnet[] = { 255,255,255,0};
byte gateway[] = { 192,168,1,254};

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

String readString;
String prevReadString;
String jsonData;

String offTime;
String onTime;
String freq;

void setup() {
  // disable SPI for the SD
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
        }
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("<a href=\"/SignalOn:5000;\"\"><button>Turn on signal</button></a>");
          client.println("<a href=\"/SignalOff:5000;\"\"><button>Turn off signal</button></a>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
//    Serial.println(readString);
    if (readString != prevReadString){
      int ind1 = readString.indexOf('{');
      int ind2 = readString.indexOf('}');
      jsonData = readString.substring(ind1, ind2+1);
      if (readString.indexOf("ChangePowerFreq") > -1){
        int ind1 = readString.indexOf("ChangePowerFreq:");
        int ind2 = readString.indexOf(";", ind1);
        freq = readString.substring(ind1+16, ind2);
        jsonData = "{\"operation\":\"change_power_frequency\", \"frequency\" :\"" + freq + "\"}";
      }
      else if (readString.indexOf("ChangeMixerFreq") > -1){
        int ind1 = readString.indexOf("ChangeMixerFreq:");
        int ind2 = readString.indexOf(";", ind1);
        freq = readString.substring(ind1+16, ind2);
        jsonData = "{\"operation\":\"change_mixer_frequency\", \"frequency\" :\"" + freq + "\"}";
      }
      else if (readString.indexOf("SignalOff") > -1){
        int ind1 = readString.indexOf("SignalOff:");
        int ind2 = readString.indexOf(";", ind1);
        offTime = readString.substring(ind1+10, ind2);
        jsonData = "{\"operation\":\"stop_power\",\"stop_time\" :\"" + offTime + "\"}";
      }
      else if (readString.indexOf("SignalOn") > -1){
        int ind1 = readString.indexOf("SignalOn:");
        int ind2 = readString.indexOf(";", ind1);
        onTime = readString.substring(ind1+9, ind2);
        jsonData = "{\"operation\":\"start_measurement\",\"measure_time\" :\"" + onTime + "\"}";
      }
      if (jsonData != "{}"){
        Serial.println(jsonData);
      }
      prevReadString = readString;
      readString = "";
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    readString="";  
  }
}

void parseInfo (){

}
