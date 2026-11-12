#ifndef Walking_H
#define Walking_H
#include <Arduino.h>

extern const int Servo;   // power pin of the PCA9685 servo driver

// Powers the servo driver, stands up and starts the walking task
void WalkingBegin();

// Keys from the web page: w a s d = walk, q e = turn
void WalkingKeyPressed(char Key);
void WalkingKeyReleased(char Key);

#endif
