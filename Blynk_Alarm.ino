#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "2c361cc388594f3e8088c65a1d3222b3";

SimpleTimer timer;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "CEREBRO_GUEST";
char pass[] = "17flyonWater";

int state;
int count=0;
int flag=1;

WidgetLED LED1 (V1);
WidgetLED motionLED(V2);

void myTimerEvent()
{
  count = count+1;
  
  if(count==5)
  {
    motionLED.off(); // Reset motionLED every 5 secs
    digitalWrite(D2, LOW);
    digitalWrite(D6, LOW);
    count=0;
    flag = 1;
  }

  int movementDectected = digitalRead(D7);  // Get PIR sensor reading
  if (movementDectected) // You can write any condition to trigger e-mail sending
  {
    if (flag == 1)
    {
        Serial.println("Motion Dectected"); // This can be seen in the Serial Monitor
        Blynk.email("Subject: PIR Sensor", "Movement detected at home!");
        Blynk.notify("WARNING: Motion detected at home!");
        motionLED.on();  // Turn on motionLED on Blynk app
        digitalWrite(D2, HIGH);  // Turn on motion LED on breadboard
        digitalWrite(D6, HIGH);  // Trun on buzzer
        flag = 2;     
    }
  }
}

BLYNK_WRITE(V0)
{
  state = param.asInt();
  if (state == 1){
    Serial.println("alarm ON");
    digitalWrite(D4, LOW);
    LED1.on();
  }
  else {
    Serial.println("alarm OFF");
    digitalWrite(D4, HIGH);
    LED1.off();
    motionLED.off(); 
    digitalWrite(D2, LOW);
    digitalWrite(D6, LOW);
    flag = 2;
    }
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(D4, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D6, OUTPUT); // don't use D1 for buzzer on wemos›
  pinMode(D7, INPUT);
  timer.setInterval(1000L, myTimerEvent);  // Interrupt every sec
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates SimpleTimer
}
