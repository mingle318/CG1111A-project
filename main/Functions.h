#pragma once
#include <stdint.h>

// Declarations shared by the sketch tabs.
void setWheels(int leftPower, int rightPower);
void setMotion(int leftDirection, int rightDirection, bool turning);
bool irTableValid();
int readIrRaw();
double getIRDist();
float ultrasonicDistanceCm();
int getClosestColour();
void toggleDecoderPin(int Y_pin);
int getAvgReading(int times);
void resetWalls(uint32_t now);
void updateDistanceSensors();
void driveStraight();
void printCalibration();
void stopRobot();
bool blackDetected();
void addStep(int leftDirection, int rightDirection,
             uint32_t duration, bool turning);
void startPlan();
void turnLeft90();
void turnRight90();
void turn180();
void uTurnLeft(); 
void uTurnRight();
void solveChallenge();
void beginApproach(uint32_t now, bool mustClearMarker);
void finishStep(uint32_t now);
void updateManeuver(uint32_t now);
void setup();
void loop();
