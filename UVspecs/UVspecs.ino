
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(3,255);
  
  float valA1 = analogRead(A1);
  float voltageA1 = valA1*5/1024;
  
  float valA0 = analogRead(A0);
  float voltageA0 = valA0*5/1024;
  
  float current = voltageA1/10;

  float valA2 = analogRead(A2);
  float voltageA2 = valA2*5/1024;
  
  Serial.write("Voltage A0: ");
  Serial.print(voltageA0); 
  Serial.write(" V");
 
  Serial.write(", Current: ");
  Serial.print(current);
  Serial.write(" mA");

  Serial.write(" Voltage A2: ");
  Serial.print(voltageA2); 
  Serial.write(" V");
  
  Serial.println();
  Serial.println();

  
  delay(1000);
}
