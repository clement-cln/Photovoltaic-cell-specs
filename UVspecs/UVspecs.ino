float voltageA0, voltageA1, voltageA2; // valeurs analogiques : AO pour la tension de sortie de la cellule Pv, A1 courant de sortie de la cellule PV, A2 tension de sortie du Buck Boost
float valA0, valA1, valA2; // valeurs numeriques lues sur ADC 

int voltageT1, voltageT2; // voltageT1 : tension precedante, voltageT2 tension actuelle


int pin1 = 7;
int pin2 = 8;
int pinPWM = 5;
float maxVoltage = 1.68;
float voltageValues[] = {0.8, 0.8, 0.8, 0.8, 0.8}; // tableau des dernieres tensions mesurées; valeur moyenne initiale = 0.8V, correspond à l'eclairement moyen en classe
int numberOfValues = 5; // nombre de valeur de voltageValues[]
bool sens = true; // sens du moteur 
int state1 = 1; 
int state2 = 0;
float goalValue = 0; // valeur de tension à atteindre


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
  
  valA1 = analogRead(A1); // on lit avec l'ADC le courant en sortie de la cellule PV 
  voltageA1 = valA1*5/1024; // conversion pour avoir une valeur analogique
  
  valA0 = analogRead(A0); // on lit la tension de sortie de la cellule PV
  voltageA0 = valA0*5/1024;
  
  float current = voltageA1/10; 

  valA2 = analogRead(A2); // on lit la tension en sortie du Buck
  voltageA2 = valA2*5/1024;

   if(voltageA2 < averageVoltageValue()){ // on compare la valeur de tension A2 à la tension moyenne pour savoir si l'on doit activer le moteur 
    goalValue = averageVoltageValue(); // la valeur à atteindre est la valeur moyenne
    lookingForMPPT(); // on lance la recherche du MPPT
  }
  else{
    //Do nothing, the solar panel is fine
  }

  updateVoltageValues(voltageA2); // on met à jour la valeur moyenne en prenant en compte la tension A2

 // on affiche les valeurs des tensions et courant sur le terminal
  
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

// on lit la valeur de tension A2 en sortie du Buck
float readA2(){
  valA2 = analogRead(A2);
  voltageA2 = valA2*5/1024;
  return voltageA2;
}

// on met à jour le tableau des tensions pour prendre en compte la valeur courante
void updateVoltageValues(float newVoltage){
  for(int i = 0; i< numberOfValues - 1; i++){
    voltageValues[i] = voltageValues[i+1];
  }
  voltageValues[numberOfValues - 1] = newVoltage;
  
}

// on retourne la valeur moyenne de la tension en fonction des dernieres valeurs 
float averageVoltageValue(){
  float average = 0;
  for(int i = 0; i< numberOfValues; i++){
    average += voltageValues[i];
  }
  average = average / numberOfValues;
  return average;
}

//mise en route du moteur 
void actionMotor(int currentSens){
  //on fixe le sens 
  if(currentSens){
    state1 = 1;
    state2 = 0;
  }
  else{
    state1 = 0;
    state2 = 1;
  }

  // on active la PWM
  analogWrite(pinPWM, 100);
  digitalWrite(pin1, state1);
  digitalWrite(pin2, state2);
  delay(100);
}

//arret du moteur
void stopMotor(){
  analogWrite(pinPWM, 0);
}

// lecture de la tension actuelle et mise à jour de la tension precedente 
void readAndUpdateA2(){
  voltageT1 = voltageT2; // mise à jour de la tension precedente 
  voltageT2 = readA2(); // lecture de la tension actuelle 
}

//Recherche du MPPT
void lookingForMPPT(){
  voltageT1 = readA2(); // on initialise la "tension precedente" 
  actionMotor(sens);
  voltageT2 = readA2(); // on lit la tension actuelle 

  while( voltageT2 > voltageT1 ){ // tant que la tension actuelle est superieure à la tension precedente on active le moteur dans un sens et on met à jour les valeurs actuelle et precedente
    actionMotor(sens);
    readAndUpdateA2();
  }

  sens = !sens; // on change de sens de rotation du moteur 

  while( voltageT2 > voltageT1 ){  // tant que la tension actuelle est superieure à la tension precedente on active le moteur dans le nouveau sens et on met à jour les valeurs actuelle et precedente
    actionMotor(sens); 
    readAndUpdateA2();
  }

  stopMotor(); // la tension atteinte est la tension maximale possible, on s'arrete 
  
}
