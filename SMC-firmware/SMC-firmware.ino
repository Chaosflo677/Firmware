const float V_REF = 5.0;     // Analog reference voltage (e.g., 5V or 3.3V)
const float R_BITS = 10.0;   // ADC resolution (bits)
const float ADC_STEPS = (1 << int(R_BITS)) - 1; // Number of steps (2^R_BITS - 1)
void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
}
void calc_v(){
  int rawValue = analogRead(A3);
  float voltage = (rawValue / ADC_STEPS) * V_REF; // Convert to voltage
  Serial.println(voltage);
}
void dlay(int i){
  for(int a = 0;a<=i;a++)
  {
  delay(850);
  digitalWrite(13,HIGH);
  delay(150);
  digitalWrite(13,LOW);
  }
}
void loop() {
  digitalWrite(8,LOW);
  digitalWrite(11,HIGH);
  dlay(1);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
  digitalWrite(10,HIGH);
  digitalWrite(9,HIGH);
  dlay(20);
  digitalWrite(11,HIGH);
  digitalWrite(10,LOW);
  dlay(1);
  digitalWrite(12,HIGH);
  digitalWrite(11,LOW);
  digitalWrite(9,LOW);
  digitalWrite(8,HIGH);
  dlay(20);
}