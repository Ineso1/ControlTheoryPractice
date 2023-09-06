//Definicion de fiunciones control de posicion
#include "PositionMotorControl.h"

PositionMotorControl* motorInstance;

PositionMotorControl::PositionMotorControl(int motorPin1, int motorPin2, int motorPin3, int encoderAPin, int encoderBPin, float maxDistanceMM, float pulsesPerRevolution, float mmPerRevolution)
  : motorPin1(motorPin1), motorPin2(motorPin2), motorPin3(motorPin3), encoderAPin(encoderAPin), encoderBPin(encoderBPin), maxDistanceMM(maxDistanceMM), pulsesPerRevolution(pulsesPerRevolution), mmPerRevolution(mmPerRevolution) {
    targetPosition = 0;
    currentPosition = 0.0;
    motorSpeed = 0;
    error = 0;
    motorInstance = this;
}

void PositionMotorControl::init() {
  // Declaracion de salidas y entradas
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(encoderAPin, INPUT_PULLUP);
  pinMode(encoderBPin, INPUT_PULLUP);

  // Declaracion de interrupciones a encoders
  attachInterrupt(digitalPinToInterrupt(encoderAPin), handleEncoderStatic, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(encoderBPin), handleEncoderStatic, CHANGE);
}

void PositionMotorControl::control() {

  error = targetPosition - currentPosition;
  // Comparacion de direccion y velocidad
  if (error < 2) {
    motorDirection = 1; // Mover hacia adelante
    motorSpeed = 200;   // Ajusta la velocidad máxima
  } else if (error > 2) {
    motorDirection = -1; // Mover hacia atrás
    motorSpeed = 200;    // Ajusta la velocidad máxima
  } else {
    motorDirection = 0; // Detenerse si ya se alcanzó la posición
    motorSpeed = 0;
  }

  // Control del motor
  analogWrite(motorPin1, motorSpeed);
  digitalWrite(motorPin2, motorDirection == -1 ? HIGH : LOW);
  digitalWrite(motorPin3, motorDirection == 1 ? HIGH : LOW);


}

// Set de posicion del motor
void PositionMotorControl::setTargetPosition(int nPulses) {
  targetPosition = nPulses;
}

int PositionMotorControl::getCurrentPos(){
  return currentPosition;  
}

void PositionMotorControl::setCero(){
  currentPosition = 0;
  targetPosition = 0;
}

// Funcion estatica para manejo de interrupcion
static void PositionMotorControl::handleEncoderStatic() {
  motorInstance->handleEncoder(); // Llamada de funcion no estatica de la instancia
}

void PositionMotorControl::handleEncoder() {
  encoderAState = digitalRead(encoderAPin);
  encoderBState = digitalRead(encoderBPin);
  
  // Determinacion de la dirección del giro del motor
  if (encoderAState == encoderBState ) {
    currentPosition --;
    motorDirection = -1;
  }
  else
  {
    currentPosition ++;
    motorDirection = 1;
  } 
  
}
