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

  motorSpeed = 200;
  prevPwm = 0;
}

void PositionMotorControl::control()
{
  int pwmSpeed;
  error = targetPosition - currentPosition;
  
  // Comparison between direction and velocity
  if (error <= -15) {
    motorDirection = 1; // Move forward
    pwmSpeed = motorSpeed;
  }
  else if (error >= 15) {
    motorDirection = -1; // Move backward
    pwmSpeed = motorSpeed;
  }
  else {
    motorDirection = 0; // Stop
    pwmSpeed = 0;
  }

  // Motor's control
  analogWrite(motorPin1, pwmSpeed);
  digitalWrite(motorPin2, motorDirection == -1 ? HIGH : LOW);
  digitalWrite(motorPin3, motorDirection == 1 ? HIGH : LOW);
}

void PositionMotorControl::controlPID(){
  error = targetPosition - (currentPosition * 2 * 3.1416 / 493.9);
  errores[1] = errores[0];
  errores[2] = errores[1];
  errores[0] = error;
  prevPwm = motorSpeed;
  //double kp = 1.5;
  //double ki = 2;
  //double kd = 0.005;
  
  double kp = 248;
  double ki = 2300;
  double kd = 7.44;
  double a = (kp + kd/0.02) * errores[0];
  double b = (-kp + (ki*0.02) - (2*kd/0.02)) * errores[1];
  double c = (kd/0.02) * errores[2];

  int pwmSpeed = prevPwm + a + b + c;
  motorSpeed = pwmSpeed;


  // Comparison between direction and velocity
  if (error <= -1) {
    motorDirection = 1; // Move forward
  }
  else if (error >= 1) {
    motorDirection = -1; // Move backward
  }
  else {
    motorDirection = 0; // Stop
    pwmSpeed = 0;
  }

    // Motor's control
  analogWrite(motorPin1, motorSpeed);
  digitalWrite(motorPin2, motorDirection == -1 ? HIGH : LOW);
  digitalWrite(motorPin3, motorDirection == 1 ? HIGH : LOW);
  
 }

// Set de posicion del motor
void PositionMotorControl::setTargetPosition(long nPulses) {
  targetPosition = nPulses;
}

int PositionMotorControl::getCurrentPos(){
  return currentPosition;  
}

void PositionMotorControl::setCero(){
  currentPosition = 0;
  targetPosition = 0;
}

void PositionMotorControl::setVelocity(int pwm){
  motorSpeed = pwm;
}

int PositionMotorControl::getVelocity(){
  return motorSpeed;
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
