
#include "DHT.h"

// Config
const int MIN_MOISTURE_VALUE = 500;
const int MAX_NUMBER_WATERING_CYCLES = 20;
const uint8_t DHTTYPE = DHT22;

// PINS
const int DHTPIN = 2;
const uint8_t  WATERPUMP_PIN = 9;
const uint8_t  MOISTURE_SENSOR_PIN = A0;

const DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("---- Setup ----");
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  delay(1000);
}

void mainAutoPlantWatering() {
  // put your setup code here, to run once:
  pinMode(WATERPUMP_PIN, OUTPUT);
  digitalWrite(WATERPUMP_PIN, HIGH);
  Serial.println("Started");
  int moistureValue = meanValueOverMeasurements(10, MOISTURE_SENSOR_PIN, 100);
  // Check if the soil moisture is too low
  if (moistureValue > MIN_MOISTURE_VALUE) {
    moistenSoil(WATERPUMP_PIN, MOISTURE_SENSOR_PIN);
  }
  Serial.println("Sopped");
}

void moistenSoil(const uint8_t  waterPumpPin, const uint8_t  moistureSensorPin) {
  // Power on the water pump
  Serial.println("Starting waterpump");
  digitalWrite(waterPumpPin, LOW);
  boolean moistureTooLow = true;
  int cycle = 0;
  while (moistureTooLow && cycle < MAX_NUMBER_WATERING_CYCLES) {
    int moistureValue = analogRead(moistureSensorPin);
    Serial.print("Current moisture value = '");
    Serial.print(moistureValue);
    Serial.println("'");
    if (moistureValue <  MIN_MOISTURE_VALUE) {
      moistureTooLow = false;
      Serial.print("Min. moisture value reached after '");
      Serial.print(cycle);
      Serial.println("' cycles");
    }
    delay(600);
    cycle++;
  }
  Serial.print("Stopping waterpump");
  digitalWrite(waterPumpPin, HIGH);
}

int meanValueOverMeasurements(const int count, const byte moistureSensorPin, const int delayTime) {
  int v = 0;
  Serial.print("Creating mean value for '");
  Serial.print(count);
  Serial.print("' reads on moistureSensorPin '");
  Serial.print(moistureSensorPin);
  Serial.print("'. Waiting '");
  Serial.print(delayTime);
  Serial.println("' ms after every measurement");
  for (int i=0; i < count; i++) {
    int tmpValue = analogRead(moistureSensorPin);
    Serial.print("New value: '");
    Serial.print(tmpValue);
    Serial.println("'");
    v += analogRead(moistureSensorPin);
    delay(delayTime);
  }
  int meanValue = round(v / count);
  Serial.print("Mean value = '");
  Serial.print(meanValue);
  Serial.println("'");
  return meanValue;
}

