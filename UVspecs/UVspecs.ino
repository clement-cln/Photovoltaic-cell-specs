float voltageA0, voltageA1, voltageA2;
float valA0, valA1, valA2;

int voltageT1, voltageT2;


int pin1 = 7;
int pin2 = 8;
int pinPWM = 5;
float maxVoltage = 1.68;
float voltageValues[] = {0.8, 0.8, 0.8, 0.8, 0.8}; 
int numberOfValues = 5;
bool sens = true;
int state1 = 1;
int state2 = 0;
float goalValue = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinPWM, OUTPUT); // Pin PWM
  pinMode(pin1, OUTPUT); // Pin commande 1
  pinMode(pin2, OUTPUT); // Pin commande 2

}



void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(3,255);
  
  valA1 = analogRead(A1);
  voltageA1 = valA1*5/1024;
  
  valA0 = analogRead(A0);
  voltageA0 = valA0*5/1024;
  
  float current = voltageA1/10;

  valA2 = analogRead(A2);
  voltageA2 = valA2*5/1024;

   if(voltageA2 < averageVoltageValue()){
    goalValue = averageVoltageValue();
    lookingForMPPT();
  }
  else{
    //Do nothing, the solar panel is fine
  }

  updateVoltageValues(voltageA2);
  
  Serial.write("Voltage A0: ");
  Serial.print(voltageA0); 
  Serial.write(" V");
 
  Serial.write(", Current: ");
  Serial.print(current);
  Serial.write(" mA");

  Serial.write(" Voltage A2: ");
  Serial.print(voltageA2); 
  Serial.write(" V");

  Serial.write(", Average: ");
  Serial.print(averageVoltageValue()); 
  Serial.write(" V");
  
  Serial.println();
  Serial.println();

  
  delay(1000);
}

float readA2(){
  valA2 = analogRead(A2);
  voltageA2 = valA2*5/1024;
  return voltageA2;
}

void updateVoltageValues(float newVoltage){
  for(int i = 0; i< numberOfValues - 1; i++){
    voltageValues[i] = voltageValues[i+1];
  }
  voltageValues[numberOfValues - 1] = newVoltage;
  
}

float averageVoltageValue(){
  float average = 0;
  for(int i = 0; i< numberOfValues; i++){
    average += voltageValues[i];
  }
  average = average / numberOfValues;
  return average;
}

void actionMotor(int currentSens){
  if(currentSens){
    state1 = 1;
    state2 = 0;
  }
  else{
    state1 = 0;
    state2 = 1;
  }

  
  analogWrite(pinPWM, 100);
  digitalWrite(pin1, state1);
  digitalWrite(pin2, state2);
  delay(100);
}

void stopMotor(){
  analogWrite(pinPWM, 0);
}

void readAndUpdateA2(){
  voltageT1 = voltageT2;
  voltageT2 = readA2();
}

void lookingForMPPT(){
  voltageT1 = readA2();
  actionMotor(sens);
  voltageT2 = readA2();

  while( voltageT2 > voltageT1 ){
    actionMotor(sens);
    readAndUpdateA2();
  }

  sens = !sens;

  while( voltageT2 > voltageT1 ){
    actionMotor(sens);
    readAndUpdateA2();
  }

  stopMotor();
  
}
