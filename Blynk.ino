

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "KjIVfhb2xdRLEZNF6z9cl3ltGdV6FP6K"; 

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Rec";
char pass[] = "ieee1234";


// Add an Led on your blynk dashboard on Virtual Channel 1
WidgetLED led1(V1);
BlynkTimer timer;

void blinkLedWidget()
{
  if (led1.getValue()) 
  {
    led1.off();
  }
  else 
  {
    led1.on();
  }
}


// Add an accelorometer on virtual Channel 2
BLYNK_WRITE(V2)
{
  // Left Motor
  int x = param[0].asFloat();
  int y = param[1].asFloat();
  int z = param[2].asFloat();
  

  Serial.print("x: ");
  Serial.println(x);
  Serial.print("y: ");
  Serial.println(y);
  Serial.println();

  if(abs(x)<=3 && abs(y)<=3)
    stop_();
  else
  {
    if(y<-3)
      forward();
    if(y>3)
      reverse();
    if(x>3)
      left();
    if(x<-3)
      right();    
  }
}


void setup()
{
  pinMode(0, OUTPUT);                     // LED pin
  pinMode(12, OUTPUT);                    // Left motor A
  pinMode(13, OUTPUT);                    // Left motor B
  pinMode(14, OUTPUT);                    // Enable pin
  pinMode(15, OUTPUT);                    // Right motor A
  pinMode(16, OUTPUT);                    // Right motor B

  
  // Debug console
  Serial.begin(500000);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, blinkLedWidget);
}

void loop()
{
  Blynk.run();
  timer.run();
}


void forward()
{
  Serial.println("Forward");

  // Left Motor
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);


  // Enable Pin
  digitalWrite(14, HIGH);


  // Right Motor
  digitalWrite(15, HIGH);
  digitalWrite(16, LOW);
}

void reverse()
{
  Serial.println("Reverse");

  // Left Motor
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);


  // Enable Pin
  digitalWrite(14, HIGH);


  // Right Motor
  digitalWrite(15, LOW);
  digitalWrite(16, HIGH);
}

void left()
{
  Serial.println("Left");

  // Left Motor
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);


  // Enable Pin
  digitalWrite(14, HIGH);


  // Right Motor
  digitalWrite(15, LOW);
  digitalWrite(16, LOW);
}

void right()
{
  Serial.println("Right");

  // Left Motor
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);


  // Enable Pin
  digitalWrite(14, HIGH);


  // Right Motor
  digitalWrite(15, LOW);
  digitalWrite(16, HIGH);
}

void stop_()
{
  Serial.println("Stop");

  // Left Motor
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);


  // Enable Pin
  digitalWrite(14, LOW);


  // Right Motor
  digitalWrite(15, LOW);
  digitalWrite(16, LOW);
}
