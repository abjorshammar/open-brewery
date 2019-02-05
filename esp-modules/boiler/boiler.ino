#include <FS.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <MQTT.h>

// Variables

// Pins
int relayPin = D3;
int tempPin = D4;

// PID
float desiredTemp = 93;
float offset = 7;
double setpoint = desiredTemp + offset;
double input = 0;
double oldInput = 0;
double output;

// Timing
long previousMillis = 0;
long tempInterval = 1000;
long serialPrintTime = 5000;
long currentTime = 0;

int windowSize = 5000;
unsigned long windowStartTime;

// Defaults
float currentTemp = 0;
float oldTemp = 0;
boolean relayStatus = true;
boolean oldRelayStatus = false;

#define DEVICE_NAME "Open Brewery Boiler Module"

// Setup generic
const char led = LED_BUILTIN;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(tempPin);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
DeviceAddress tempProbe = {0x28, 0x2D, 0x48, 0x4E, 0x05, 0x00, 0x00, 0xF5};

// Setup PID
PID myPID(&input, &output, &setpoint, 800, 0, 0, DIRECT);

void setup()
{
    // Start serial debug
    Serial.begin(115200);

    // Setup WiFi
    WiFiManager wifiManager;
    wifiManager.autoConnect("OB-Boiler");

    //allows serving of files from SPIFFS
    SPIFFS.begin();

    // Setup relay pin
    pinMode(relayPin, OUTPUT);

    // Start up the sensors library and set the resolution to 10 bit
    sensors.begin();
    sensors.setResolution(tempProbe, 12);
}

void loop()
{
}