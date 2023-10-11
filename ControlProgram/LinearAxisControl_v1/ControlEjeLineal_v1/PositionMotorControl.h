//Header file control de posicion
#ifndef POSITIONMOTORCONTROL_H
#define POSITIONMOTORCONTROL_H

#include <Arduino.h>

class PositionMotorControl {
public:
  PositionMotorControl(int motorPin1, int motorPin2, int motorPin3, int encoderAPin, int encoderBPin, float maxDistanceMM, float pulsesPerRevolution, float mmPerRevolution);
  void init();
  void control();
  void setTargetPosition(long position);
  int getCurrentPos();
  void setCero();

private:

  long error;

  //Pines puente H
  int motorPin1;
  int motorPin2;
  int motorPin3;

  // Pines encoder A y B
  int encoderAPin;
  int encoderBPin;

  // Variables para controlar la posición actual y deseada del motor
  volatile long currentPosition;
  volatile long targetPosition;

  // Variables para el control del motor
  int motorSpeed;
  int motorDirection;

  // Variables para almacenar el estado de los encoders
  volatile int encoderAState;
  volatile int encoderBState;
  volatile int lastEncoderAState;
  volatile int lastEncoderBState;

  // Constantes mecánicas
  float maxDistanceMM;
  float pulsesPerRevolution;
  float mmPerRevolution;

  //Interrupcion cambio de estado encoders
  void handleEncoder();
  static void handleEncoderStatic();
};

#endif
