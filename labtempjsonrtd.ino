#undef min
#undef max

#include <ArduinoJson.h>
#include <Adafruit_MAX31865.h>

#define RREF 430.0
#define NUMSENSORS 3
String inputString = "";
volatile bool stringComplete = false;
bool sendtype = false;
String sensor = "";
int chan[NUMSENSORS];
int data[NUMSENSORS];
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
    for(int i = 0; i<NUMSENSORS; i++)
      rtd[i].begin(MAX31865_3WIRE);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (true)
  {
    parseJson();
    stringComplete = false;
    collectdata = true;
  }

  if(collectdata)
  {
    collect();
    collectdata = false;
    senddata = true;
  }

    if(senddata)
    {
      senddat();
      senddata = false;
    }
  
}

void parseJson()
{
    DynamicJsonBuffer jsonBuffer;

  String testinput = "{\"send\":\"true\",\"sensor\":\"thermocouple\",\"channels\":\"[0,1,2],\"recieveIP\":\"10.0.0.8\"}";

  JsonObject& root = jsonBuffer.parseObject(testinput);

  JsonArray& channels = root["channels"].asArray();

 // std::vector<int> chan;
  for (int i = 0; i < channels.size(); i++)
  {
    chan.push_back(root["channels"][i]);
  }
}
void senddat()
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["Type"] = "Thermistor";
  JsonArray& raw = root.createNestedArray("Raw");
  for(int i = 0; i < NUMSENSORS; i++)
  {
     raw.add(rtd[i].readRTD());
  }
  JsonArray& converted = root.createNestedArray("Converted");
    for(int i = 0; i < NUMSENSORS; i++)
  {
    converted.add(rtd[i].temperature(100,RREF));
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
