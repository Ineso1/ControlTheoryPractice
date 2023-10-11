//Archivo Arduino
#include "PositionMotorControl.h"


// Definición de constantes mecanicas
const float MAX_DISTANCE_MM = 1000.0;       // Distancia máxima en milímetros
const float PULSES_PER_REVOLUTION = 493.9; // Pulsos por revolución del encoder
const float MM_PER_REVOLUTION = 8.0; 

// Calcula el ángulo máximo en grados correspondiente a la distancia máxima
const float MAX_ANGLE_DEGREES = (MAX_DISTANCE_MM / MM_PER_REVOLUTION) * 360.0;
const float MAX_POT_VALUE = 1023;

//Definicion de pines del driver
const int motorPin1 = 4;  //PWM
const int motorPin2 = 5;  //Direccion
const int motorPin3 = 6;  //Direccion2

const int encoderAPin = 2;
const int encoderBPin = 3;
const int potentiometerPin = A0;

float angle_degrees = 0;
long pulses = 0;

PositionMotorControl motor(motorPin1, motorPin2, motorPin3, encoderAPin, encoderBPin, MAX_DISTANCE_MM, PULSES_PER_REVOLUTION, MM_PER_REVOLUTION);

void setup() {
  Serial.begin(9600);
  motor.init();
  motor.setVelocity(0);
}

void loop() {


  if(Serial.available()){
    String aux = Serial.readStringUntil('\n');
    if(aux=="set"){
      motor.setCero();
      angle_degrees = 0;
      
    }
    else{
      //angle_degrees = PULSES_PER_REVOLUTION * String(aux).toFloat()* 0.9076 ;
      angle_degrees = String(aux).toFloat()* 2 * 3.1416 * 0.9076 ;
      pulses = angle_degrees / MM_PER_REVOLUTION ;
      Serial.print(angle_degrees);
      Serial.print(" -> ");
    }
  }
  
  Serial.println(motor.getCurrentPos());
  Serial.print(" -> ");
  Serial.print(pulses);
  Serial.print(" -> cv ");
  Serial.println(motor.getVelocity());
  //Serial.print(" -> nv");
  //Serial.print(motor.getVelocity());
  

  // Establecer la posición objetivo del motor en pulsos
  motor.setTargetPosition(pulses);

  //Algoritmo de control
  motor.controlPID();
}

float readPot(int potPin){
  // Leer el valor del potenciómetro
  int potValue = analogRead(potentiometerPin);
  
  // Calculo del ángulo objetivo en grados
  float angle_degrees = map(potValue, 0, MAX_POT_VALUE, 0, MAX_ANGLE_DEGREES);

  // Calculo de cantidad de pulsos objetivo en función del ángulo
  float pulses_target = angle_degrees * (PULSES_PER_REVOLUTION / 360.0);

  Serial.println(pulses_target);

  return pulses_target;
}
