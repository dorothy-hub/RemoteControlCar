// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "HUAWEI P20";
const char* password = "peepeepoopoo123";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String motorLeftState = "off";
String motorRightState = "off";
String reverseState = "off";

// Assign output variables to GPIO pins
const int output12 = 12;
const int output13 = 13;
const int output14 = 14;
const int output15 = 15;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output12, OUTPUT);
  pinMode(output13, OUTPUT);
  pinMode(output14, OUTPUT);
  pinMode(output15, OUTPUT);

  // Set outputs to LOW
  digitalWrite(output13, LOW);
  digitalWrite(output15, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("Motor Right ON");
              motorRightState = "on";
              reverseState = "off";
              digitalWrite(output14, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("Motor Right OFF");
              motorRightState = "off";
              reverseState = "off";
              digitalWrite(output14, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("Motor Left ON");
              motorLeftState = "on";
              reverseState = "off";
              digitalWrite(output12, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("Motor Left OFF");
              motorLeftState = "off";
              reverseState = "off";
              digitalWrite(output12, LOW);
            }

            // Reverse
            if (header.indexOf("GET /6/on") >= 0) {
              reverseState = "on";
              motorLeftState = "off";
              motorRightState = "off";
              digitalWrite(output12, LOW);
              digitalWrite(output13, HIGH);
              digitalWrite(output14, LOW);
              digitalWrite(output15, HIGH);
            } else {
              reverseState = "off";
              digitalWrite(output13, LOW);
              digitalWrite(output15, LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");

            // Display current state, and ON/OFF buttons for GPIO 5
            client.println("<p>Motor Left - " + motorRightState + "</p>");
            // If the output5State is off, it displays the ON button
            if (motorRightState == "off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">ON</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 4
            client.println("<p>Motor Right - " + motorLeftState + "</p>");
            // If the output4State is off, it displays the ON button
            if (motorLeftState == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("<p>Reverse</p>");
            if (reverseState == "off") {
              client.println("<p><a href=\"/6/on\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/6/off\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
