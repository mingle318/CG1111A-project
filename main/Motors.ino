void setWheels(int leftPower, int rightPower) {
  leftMotor.run(LEFT_FORWARD_SIGN * constrain(leftPower, -255, 255));
  rightMotor.run(RIGHT_FORWARD_SIGN * constrain(rightPower, -255, 255));
}

void setMotion(int leftDirection, int rightDirection, bool turning) {
  int leftPower = turning ? TURN_LEFT : DRIVE_LEFT;
  int rightPower = turning ? TURN_RIGHT : DRIVE_RIGHT;
  setWheels(leftDirection * leftPower, rightDirection * rightPower);
}

void driveStraight() {
  int correction = (int)roundf(steering);
  setWheels(constrain(DRIVE_LEFT - correction, 0, 255),
            constrain(DRIVE_RIGHT + correction, 0, 255));
}

void stopRobot() {
  setWheels(0, 0);
  celebrate();
  state = STOPPED;
}
