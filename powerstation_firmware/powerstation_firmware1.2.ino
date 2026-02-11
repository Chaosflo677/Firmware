#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
const float V_REF = 5.0;     // Analog reference voltage (e.g., 5V or 3.3V)
const float R_BITS = 10.0;   // ADC resolution (bits)
const float ADC_STEPS = (1 << int(R_BITS)) - 1; // Number of steps (2^R_BITS - 1)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);
byte degree[8] = {
0b00110,
0b01001,
0b01001,
0b00110,
0b00000,
0b00000,
0b00000,
0b00000
};
byte batt[8] = {
0b01110,
0b01010,
0b10001,
0b10001,
0b10001,
0b10001,
0b10001,
0b11111
};
byte charging[8] = {
0b00100,
0b01100,
0b01100,
0b11111,
0b11111,
0b00110,
0b00110,
0b00100
};
void setup() {
  lcd.init();
  Serial.begin(9600);
  sensors.begin();
  pinMode(4, OUTPUT);
  digitalWrite(4,HIGH);
  lcd.createChar(0, degree);
  lcd.createChar(1, batt);
  lcd.createChar(2, charging);
}
float v=0;
char power[4] = "N.A";
char status[9] = "ok";
bool battery_connected = false;
bool devmode = false;
void disconnect_battery() {
  if(battery_connected){
  Serial.println("disconnecting power");
  battery_connected = false;
  digitalWrite(4,LOW);
  }
}
void connect_battery() {
  if(!battery_connected){
    Serial.println("connecting power");
    strcpy(status, "ok");
    battery_connected = true;
    digitalWrite(4,HIGH);
  }
}
void calc_v(){
  int rawValue = analogRead(A3);
  float voltage = (rawValue / ADC_STEPS) * V_REF; // Convert to voltage
  v=voltage*4;
  if(status=="debug"){
    Serial.println(v);
  }
}
void calc_power(){
  if(v>12.6){strcpy(power,"110");}
  if(v<12.6){strcpy(power,"100");}
  if(v<12.1){strcpy(power," 76");}
  if(v<11.6){strcpy(power," 52");}
  if(v<11.5){strcpy(power," 40");}
  if(v<11.1){strcpy(power," 11");}
  if(v<10.8){strcpy(power,"  0");}
  if(v==0){strcpy(power,"N.A");}
  
}

void loop() {
  sensors.requestTemperatures();
  lcd.backlight();
  int temp(sensors.getTempCByIndex(0));
  calc_v();
  calc_power();
  if(temp>=45.00&&devmode==false){
    strcpy(status,"overheat");
    disconnect_battery();
  }
  else{
    connect_battery();
  }
  //Nachricht ausgeben
  if(status=="debug"){
    Serial.println("Temperature is: " + String(sensors.getTempCByIndex(0)) + "°C");
    Serial.println(floor(temp));
  }
  lcd.setCursor(0, 0);
  lcd.print(power);
  lcd.setCursor(3, 0);
  lcd.print("%");
  lcd.setCursor(4, 0);
  lcd.write(byte(1));
  lcd.setCursor(12, 0);
  lcd.print(String(temp));
  lcd.setCursor(14, 0);
  lcd.write(byte(0));
  lcd.setCursor(15, 0);
  lcd.print("C");
  if(devmode == true){
    lcd.setCursor(7, 0);
    lcd.print("DEV");
    lcd.setCursor(2, 1);
    lcd.print("OHP disabled");
  }
  else{
    lcd.setCursor(0, 1);
    lcd.print("status:");
    lcd.setCursor(8, 1);
    lcd.print(status);
  }
  
  delay(1000);
}