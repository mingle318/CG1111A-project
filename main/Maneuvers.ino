void addStep(int leftDirection, int rightDirection,
             uint32_t duration, bool turning) {
  if (stepCount >= 3) return;
  steps[stepCount].leftDirection = leftDirection;
  steps[stepCount].rightDirection = rightDirection;
  steps[stepCount].duration = duration;
  steps[stepCount].turning = turning;
  ++stepCount;
}

void startPlan() {
  setWheels(0, 0);
  stepIndex = 0;
  pausing = true;
  pauseStartedAt = millis();
  state = MANEUVER;
}

void turnLeft90() {
  stepCount = 0;
  addStep(-1, 1, LEFT_90_MS, true);
  startPlan();
}

void turnRight90() {
  stepCount = 0;
  addStep(1, -1, RIGHT_90_MS, true);
  startPlan();
}

void turn180() {
  stepCount = 0;
  double distance_to_right = ultrasonicDistanceCm();
  if(distance_to_right == -1.0 || distance_to_right > 10.5) {
    addStep(1, -1, TURN_180_MS_RIGHT, true);
  } else {  
    addStep(-1, 1, TURN_180_MS_LEFT, true);
  }
  startPlan();
}

void uTurnLeft() {
  stepCount = 0;
  addStep(-1, 1, LEFT_90_MS, true);
  addStep(1, 1, GRID_MS_LEFT, false);
  addStep(-1, 1, LEFT_90_MS, true);
  startPlan();
}

void uTurnRight() {
  stepCount = 0;
  addStep(1, -1, RIGHT_90_MS, true);
  addStep(1, 1, GRID_MS_RIGHT, false);
  addStep(1, -1, RIGHT_90_MS, true);
  startPlan();
}

enum Action pre = U_LEFT;

void solveChallenge() {
  steering = 0;
  int idx = guessColorByValue();
  enum Action action = ACTION[idx];
  setColor(COLOUR_TARGET[idx]);
  switch (action) {
    case LEFT_90:  turnLeft90();  break;
    case RIGHT_90: turnRight90(); break;
    case TURN_180: turn180();    break;
    case U_LEFT:   uTurnLeft();  break;
    case U_RIGHT:  uTurnRight(); break;
    default: stopRobot(); break;
  }
}

void beginApproach(uint32_t now, bool mustClearMarker) {
  state = APPROACH;
  motionStartedAt = now;
  resetWalls(now);
}

void finishStep(uint32_t now) {
  setWheels(0, 0);
  ++stepIndex;
  pausing = true;
  pauseStartedAt = now;
}

void updateManeuver(uint32_t now) {
  if (pausing) {
    if ((uint32_t)(now - pauseStartedAt) < SETTLE_MS) return;

    if (stepIndex >= stepCount) {
      beginApproach(now, true);
      return;
    }

    pausing = false;
    motionStartedAt = now;
  }

  Step &step = steps[stepIndex];
  uint32_t elapsed = now - motionStartedAt;

  if (elapsed >= step.duration) {
    finishStep(now);
    return;
  }

  if (step.turning) {
    setMotion(step.leftDirection, step.rightDirection, true);
  } else {
    driveStraight();
  }
}
