#pragma once
#include <stdint.h>

// Declarations shared by the sketch tabs.
void setWheels(int leftPower, int rightPower);
void setMotion(int leftDirection, int rightDirection, bool turning);
int readIrRaw();
double getIRDist();
float ultrasonicDistanceCm();
void toggleDecoderPin(int Y_pin);
int getAvgLDRReading(int times);
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
void setColor(const int c[3]);
void doRest(int *i, const int score[], int beat_duration, int total_notes);
void doPlayNote(int *i, const int score[], int beat_duration, int total_notes);
void readScore(int score[]);
void celebrate();
void setup();
void loop();
