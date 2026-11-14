#include <MeMCore.h>
#include <math.h>
#include <stdio.h>
#include "Config.h"
#include "Functions.h"

MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);
MeLineFollower lineSensor(PORT_2);

enum RobotState { WAITING, APPROACH, MANEUVER, STOPPED, CALIBRATING };
RobotState state = WAITING;

struct Step {
  int8_t leftDirection;
  int8_t rightDirection;
  uint32_t duration;
  bool turning;
};

Step steps[3];
uint8_t stepCount = 0, stepIndex = 0;
bool pausing = false;
uint32_t poweredOnAt = 0, motionStartedAt = 0;
uint32_t pauseStartedAt = 0;


uint32_t lastWallAt = 0;
float steering = 0;

void setup() {
  setWheels(0, 0);
  //Serial.begin(115200);
  pinMode(IR_PIN, INPUT);
  pinMode(US_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(DECODER.A_pin, OUTPUT);
  pinMode(DECODER.B_pin, OUTPUT);

  state = WAITING;
  stepCount = stepIndex = 0;
  pausing = false;
  poweredOnAt = millis();
  resetWalls(poweredOnAt);
}

void loop() {
  if (state == STOPPED) return;
  const uint32_t now = millis();

  if(state == CALIBRATING) {
    delay(3000);
    int idx = guessColorByValue();
    setColor(COLOUR_TARGET[idx]);
    printCalibration();
    return;
  }

  if (state == MANEUVER) {
    updateManeuver(now);
    return;
  }
  if (state == WAITING) {
    if ((uint32_t)(now - poweredOnAt) < START_DELAY_MS) return;
    beginApproach(now, false);
  }

  if (state == APPROACH) {
    setColor(COLOUR_TARGET[6]);
    if (blackDetected()) {
      setWheels(0, 0);
      solveChallenge();
      return;
    }
    updateDistanceSensors();
    uint32_t afterRead = millis();
    // Check black again after the bounded ultrasonic wait.
    if (blackDetected()) {
      setWheels(0, 0);
      solveChallenge();
      return;
    }
    uint32_t elapsed = afterRead - motionStartedAt;
    if (MAX_RUN_MS > 0 && elapsed >= MAX_RUN_MS) {
      stopRobot();
      return;
    }
    driveStraight();
  }
}
