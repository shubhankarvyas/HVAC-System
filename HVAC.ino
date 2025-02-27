#include <DHT.h> 
#include <LiquidCrystal.h> 
 
#define DHTPIN 13    // Pin connected to DHT11 sensor 
#define DHTTYPE DHT11   // DHT11 sensor type 
 
DHT dht(DHTPIN, DHTTYPE); 
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
 
const int redPin = 10; 
const int bluePin = 9; 
const int whitePin = 8; 
const int yellowPin = 7; 
const int buzzerPin = 6; 
 
const int humiditySetPin = A0; 
const int temperatureSetPin = A1; 
const int volumeSetPin = A2; 
const int powerPin = A3; // Changed to an analog pin since it's used as an input 
 
int humidityLow = 25; 
int humidityHigh = 75; 
int tempLow = 5; 
int tempHigh = 45; 
 
void setup() { 
  lcd.begin(16, 2); 
  Serial.begin(115200); 
 
  pinMode(redPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
  pinMode(whitePin, OUTPUT);  
  pinMode(yellowPin, OUTPUT); 
  pinMode(buzzerPin, OUTPUT); 
  pinMode(powerPin, INPUT); 
 
  dht.begin(); 
} 
 
void loop() { 
  float humiditySet = map(analogRead(humiditySetPin), 0, 1023, 30, 70); 
  float temperatureSet = map(analogRead(temperatureSetPin), 0, 1023, 10, 40); 
  int volumeSet = map(analogRead(volumeSetPin), 0, 1023, 0, 255); 
 
  delay(2000); // Wait a few seconds between measurements. 
 
  float humidity = dht.readHumidity(); 
  float temperature = dht.readTemperature(); 
 
  // Check if any reads failed and exit early (to try again). 
  if (isnan(humidity) || isnan(temperature)) { 
    Serial.println(F("Failed to read from DHT sensor!")); 
    return; 
  } 
 
  displayValues(temperature, temperatureSet, humidity, humiditySet); 
  hvacAction(temperature, temperatureSet, humidity, humiditySet); 
  hvacAlarm(temperature, humidity); 
} 
 
void hvacAlarm(float temp, float humidity) { 
  if (temp < tempLow || temp > tempHigh || humidity < humidityLow || humidity > humidityHigh) { 
    tone(buzzerPin, 1000); 
  } else { 
    noTone(buzzerPin); 
  } 
}  
void displayValues(float temp, float tempSet, float humidity, float humiditySet) { 
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("Temp: "); 
  lcd.print(temp); 
  lcd.print("C"); 
  lcd.print("  "); 
  lcd.print("Humidity: "); 
  lcd.print(humidity); 
  lcd.print("%"); 
  lcd.setCursor(0, 1); 
  lcd.print("Set Temp: "); 
  lcd.print(tempSet); 
  lcd.print("C"); 
  lcd.print("  "); 
  lcd.print("Set Humidity: "); 
  lcd.print(humiditySet); 
  lcd.print("%"); 
} 
 
void hvacAction(float temp, float tempSet, float humidity, float humiditySet) { 
  digitalWrite(bluePin, temp > tempSet); 
  digitalWrite(redPin, temp < tempSet); 
  digitalWrite(whitePin, humidity > humiditySet); 
  digitalWrite(yellowPin, humidity < humiditySet); 
} 
