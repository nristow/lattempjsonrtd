#undef min
#undef max

#include <ArduinoJson.h>
#include <Adafruit_MAX31865.h>

#define RREF 430.0

String inputString = "";
volatile bool stringComplete = false;
bool sendtype = false;
String sensor = "";
int chan[3];
double data[50];
bool collectdata = false;
bool senddata = false;

//Adafruit_MAX31865 rtd[3];

Adafruit_MAX31865 max3 = Adafruit_MAX31865(4, 5, 6, 7);
Adafruit_MAX31865 max2 = Adafruit_MAX31865(3, 5, 6, 7);
Adafruit_MAX31865 max1 = Adafruit_MAX31865(2, 5, 6, 7);

Adafruit_MAX31865 rtd[] = {max1,max2,max3};
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    for(int i = 0; i<3; i++)
      rtd[i].begin(MAX31865_3WIRE);
    //max.begin(MAX31865_3WIRE);
    //max2.begin(
}

void loop() {
  // put your main code here, to run repeatedly:

}

void senddat()
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& sendto = root.createNestedArray("Thermocouple");
  for(int i = 0; i < 3; i++)
  {
    sendto.add(data[i]);
  }

  root.printTo(Serial);
  Serial.println();
}

  void serialEvent() 
  {
      while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
