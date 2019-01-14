
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  float valA1 = analogRead(A1);
  float voltageA1 = valA1*5/1.024;
  float valA0 = analogRead(A0);
  float voltageA0 = valA0*5/1024;
  float current = voltageA1/24;
  Serial.write("Voltage A0: ");
  Serial.print(voltageA0); 
  Serial.write(" V");

  Serial.write(" Voltage A1: ");
  Serial.print(voltageA1); 
  Serial.write(" V");
 
  Serial.write(", Current: ");
  Serial.print(current);
  Serial.write(" mA");
  Serial.println();
  Serial.println();
  
  delay(1000);
}
