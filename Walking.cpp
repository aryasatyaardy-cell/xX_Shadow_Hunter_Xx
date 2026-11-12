#include "Walking.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// The PCA9685 uses its own I2C bus (Wire1), so it does not clash with the
// vibration sensor, which keeps using Wire on pins 38 and 39
const int ServoSDA = 8;
const int ServoSCL = 9;

#define ServoMin 102
#define ServoMax 512

#define Shoulder1X 0
#define Shoulder1Y 1
#define Knee1 2

#define Shoulder2X 4
#define Shoulder2Y 5
#define Knee2 6

#define Shoulder3X 8
#define Shoulder3Y 9
#define Knee3 10

#define Shoulder4X 12
#define Shoulder4Y 13
#define Knee4 14

// Leg 1 = back left, Leg 2 = front left, Leg 3 = front right, Leg 4 = back right

const int DownShoulder = 70;
const int DownKnee = 20;

const int UpShoulder = 80;
const int UpKnee = 25;

// Walking settings
const int Neutral = 90;      // leg pointing diagonally outward

// Step size of each leg (degrees each side of neutral)
const int Stride1 = 40;
const int Stride2 = 30;
const int Stride3 = 40;
const int Stride4 = 30;

const int LiftTime = 120;    // ms to lift or lower one leg
const int SwingTime = 250;   // ms to swing one leg
const int PushTime = 600;    // ms for the body push (all legs together)
const int UpdateTime = 10;   // ms between small servo steps

// w = forward, s = backward, a = left, d = right
// q = turn left, e = turn right, x = stop
static volatile char WalkingDirection = 'x';
static volatile bool WalkingReady = false;
static char ActiveDirection = 'x';
static bool Centered = true;

static int CurrentAngle[16];   // last angle sent to every channel (-1 = unknown)

static Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire1);

void SetServoAngle(uint8_t channel, int angle) {
  angle = constrain(angle, 0, 180);
  CurrentAngle[channel] = angle;
  int tick = map(angle, 0, 180, ServoMin, ServoMax);
  pwm.setPWM(channel, 0, tick);
}

//////////////////////////////////////////////////////////////////

// Move up to 4 servos together, in small eased steps
void MoveServos(int Count, uint8_t Channel[], int Target[], int Duration) {
  int Start[4];

  for (int i = 0; i < Count; i++) {
    if (CurrentAngle[Channel[i]] < 0) {
      Start[i] = Target[i];
    } else {
      Start[i] = CurrentAngle[Channel[i]];
    }
  }

  int Steps = max(1, Duration / UpdateTime);

  for (int s = 1; s <= Steps; s++) {
    float t = (float)s / Steps;
    t = t * t * (3 - 2 * t);   // slow start, slow stop

    for (int i = 0; i < Count; i++) {
      SetServoAngle(Channel[i], Start[i] + (Target[i] - Start[i]) * t + 0.5);
    }

    delay(UpdateTime);
  }
}

void MoveLegHeight(uint8_t ShoulderY, uint8_t Knee, int ShoulderAngle, int KneeAngle) {
  uint8_t Channel[2] = {ShoulderY, Knee};
  int Target[2] = {ShoulderAngle, KneeAngle};
  MoveServos(2, Channel, Target, LiftTime);
}

void MoveLegSwing(uint8_t ShoulderX, int Angle) {
  uint8_t Channel[1] = {ShoulderX};
  int Target[1] = {Angle};
  MoveServos(1, Channel, Target, SwingTime);
}

// Lift one leg, swing it, put it down
void StepLeg(uint8_t ShoulderY, uint8_t Knee, uint8_t ShoulderX, int Angle) {
  MoveLegHeight(ShoulderY, Knee, UpShoulder, UpKnee);
  MoveLegSwing(ShoulderX, Angle);
  MoveLegHeight(ShoulderY, Knee, DownShoulder, DownKnee);
}

// All four legs on the ground, push the body together
void PushBody(int Angle1, int Angle2, int Angle3, int Angle4) {
  uint8_t Channel[4] = {Shoulder1X, Shoulder2X, Shoulder3X, Shoulder4X};
  int Target[4] = {Angle1, Angle2, Angle3, Angle4};
  MoveServos(4, Channel, Target, PushTime);
}

//////////////////////////////////////////////////////////////////

// True when the key on the web page changed, the step is cut short
bool KeyChanged() {
  return WalkingDirection != ActiveDirection;
}

// One full step. Each Sign is 1 or -1:
//  1 = leg swings toward a higher angle, then pushes back to a lower angle
// -1 = leg swings toward a lower angle, then pushes back to a higher angle
void Walk(int Sign1, int Sign2, int Sign3, int Sign4) {
  int Swing1 = Neutral + (Sign1 * Stride1);
  int Swing2 = Neutral + (Sign2 * Stride2);
  int Swing3 = Neutral + (Sign3 * Stride3);
  int Swing4 = Neutral + (Sign4 * Stride4);

  StepLeg(Shoulder1Y, Knee1, Shoulder1X, Swing1);
  if (KeyChanged()) return;

  StepLeg(Shoulder2Y, Knee2, Shoulder2X, Swing2);
  if (KeyChanged()) return;

  StepLeg(Shoulder4Y, Knee4, Shoulder4X, Swing4);
  if (KeyChanged()) return;

  StepLeg(Shoulder3Y, Knee3, Shoulder3X, Swing3);

  delay(100);

  // push: every leg goes to the mirror of its swing angle around neutral
  PushBody(2 * Neutral - Swing1, 2 * Neutral - Swing2, 2 * Neutral - Swing3, 2 * Neutral - Swing4);

  delay(100);
}

//                          leg:  1   2   3   4
void WalkForward()  { Walk(      1,  1, -1, -1); }   // toward legs 2 and 3
void WalkBackward() { Walk(     -1, -1,  1,  1); }   // toward legs 1 and 4
void WalkLeft()     { Walk(      1, -1, -1,  1); }   // toward legs 1 and 2
void WalkRight()    { Walk(     -1,  1,  1, -1); }   // toward legs 3 and 4
void TurnLeft()     { Walk(     -1, -1, -1, -1); }   // all legs turn the same way
void TurnRight()    { Walk(      1,  1,  1,  1); }

// Slowly bring all legs back to the middle
void CenterLegs() {
  PushBody(Neutral, Neutral, Neutral, Neutral);
}

//////////////////////////////////////////////////////////////////

void Stand() {
  delay(300);
  SetServoAngle(Shoulder1Y, UpShoulder);
  SetServoAngle(Knee1, UpKnee);
  delay(300);
  SetServoAngle(Shoulder1X, 90);
  delay(300);
  SetServoAngle(Shoulder1Y, DownShoulder);
  SetServoAngle(Knee1, DownKnee);

  delay(300);
  SetServoAngle(Shoulder3Y, UpShoulder);
  SetServoAngle(Knee3, UpKnee);
  delay(300);
  SetServoAngle(Shoulder3X, 90);
  delay(300);
  SetServoAngle(Shoulder3Y, DownShoulder);
  SetServoAngle(Knee3, DownKnee);

  delay(300);
  SetServoAngle(Shoulder2Y, UpShoulder);
  SetServoAngle(Knee2, UpKnee);
  delay(300);
  SetServoAngle(Shoulder2X, 90);
  delay(300);
  SetServoAngle(Shoulder2Y, DownShoulder);
  SetServoAngle(Knee2, DownKnee);

  delay(300);
  SetServoAngle(Shoulder4Y, UpShoulder);
  SetServoAngle(Knee4, UpKnee);
  delay(300);
  SetServoAngle(Shoulder4X, 90);
  delay(300);
  SetServoAngle(Shoulder4Y, DownShoulder);
  SetServoAngle(Knee4, DownKnee);
}

void PrintDirection(char Command) {
  if (Command == 'w') {
    Serial.println("Walking forward...");
  } else if (Command == 'a') {
    Serial.println("Walking left...");
  } else if (Command == 's') {
    Serial.println("Walking backward...");
  } else if (Command == 'd') {
    Serial.println("Walking right...");
  } else if (Command == 'q') {
    Serial.println("Turning left...");
  } else if (Command == 'e') {
    Serial.println("Turning right...");
  } else {
    Serial.println("Standing still...");
  }
}

//////////////////////////////////////////////////////////////////

// Runs on its own, so the sensors in loop() and the web server are never
// stopped by the walking delays
void WalkingTask(void *Parameter) {
  for (int i = 0; i < 16; i++) {
    CurrentAngle[i] = -1;
  }

  // Enable PCA9685 power control
  pinMode(Servo, OUTPUT);
  digitalWrite(Servo, HIGH);

  Serial.println("PCA9685 Power: ON");

  delay(5000);

  // Initialize PCA9685
  Wire1.begin(ServoSDA, ServoSCL);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  Stand();
  delay(500);

  Centered = true;
  WalkingReady = true;
  Serial.println("Walking ready");

  char LastCommand = 'x';

  while (true) {
    char Command = WalkingDirection;

    if (Command != LastCommand) {
      PrintDirection(Command);
      LastCommand = Command;
    }

    if (Command == 'x') {
      if (!Centered) {
        CenterLegs();
        Centered = true;
      }
      delay(10);
    } else {
      Centered = false;
      ActiveDirection = Command;

      if (Command == 'w') {
        WalkForward();
      }
      if (Command == 's') {
        WalkBackward();
      }
      if (Command == 'a') {
        WalkLeft();
      }
      if (Command == 'd') {
        WalkRight();
      }
      if (Command == 'q') {
        TurnLeft();
      }
      if (Command == 'e') {
        TurnRight();
      }
    }
  }
}

void WalkingBegin() {
  xTaskCreate(WalkingTask, "Walking", 8192, NULL, 2, NULL);
}

void WalkingKeyPressed(char Key) {
  if (!WalkingReady) {
    return;
  }

  Key = tolower(Key);

  if (Key == 'w' || Key == 'a' || Key == 's' || Key == 'd' || Key == 'q' || Key == 'e') {
    WalkingDirection = Key;
  }
}

void WalkingKeyReleased(char Key) {
  Key = tolower(Key);

  // only stop if the key that was released is the one we are walking with
  if (Key == WalkingDirection) {
    WalkingDirection = 'x';
  }
}
