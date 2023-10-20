////////////////////////////////////////////
//Programa identificacion motor
////Oliver hernández
///////////////////////////////////////////

#include<TimerOne.h>
///Declar pines
const int Pin_1A = 5; /// dato de direccion 
const int Pin_2A = 6; /// dato de direccion
const int Pin_EN = 4; /// Enable PWM
///////////////////////////////////////////
const int interruptPin = 3; ///// Pin DE INTERRUPCION
long unsigned int counter = 0;
int DutyCycle = 0; ///Valor Pwm DE 0-255
double Velocidad_en_RPM = 0.0; ///
float tiempo = 0.0; 

void setup() {
  pinMode(Pin_1A,OUTPUT);
  pinMode(Pin_2A,OUTPUT);
  pinMode(Pin_EN,OUTPUT);
  pinMode(interruptPin,INPUT);
//Giro del motor
  digitalWrite(Pin_1A,LOW);
  digitalWrite(Pin_2A,HIGH);
  attachInterrupt(digitalPinToInterrupt(interruptPin), counting, RISING);
  Timer1.initialize(20000);
  Timer1.attachInterrupt(ISR_RevolucionesPorMinuto);
  Serial.begin(9600);
  Serial.println("LABEL,pwm,posicion");
}

void loop() {
  DutyCycle=random(100,255); //Señal PWM aletatorio 
  analogWrite(Pin_EN,DutyCycle);
  delay(random(100,200));
}
  ////////////////////////////////////////////////
  ///Funciones
void counting(){
  ////Contador
  counter++;
  }
  void ISR_RevolucionesPorMinuto(){
    ///Velocidad(RPM)= 60*counter/resolucion*Ts
    Velocidad_en_RPM = 60*counter/(493.9*0.02);
    tiempo = tiempo + 0.02;
    Serial.print("DATA,");
    Serial.print(DutyCycle);
    Serial.print(",");
    Serial.println(Velocidad_en_RPM);
    //counter = 0;
   }
