void resetWalls(uint32_t now) {
  steering = 0;

  // Take fresh readings when straight movement begins.
  lastWallAt = now - WALL_SAMPLE_MS;
}

void updateDistanceSensors() {
  uint32_t now = millis();

  if ((uint32_t)(now - lastWallAt) < WALL_SAMPLE_MS) {
    return;
  }

  lastWallAt = now;

  float rightCm = ultrasonicDistanceCm();
  float leftCm = readIrRaw();

  // A missing echo or a very distant object is not a usable wall.
  bool rightValid = rightCm >= 2.0f && rightCm <= RIGHT_WALL_LIMIT_CM;
  if (leftCm < IR_TOO_CLOSE) {
    steering = -LEFT_CORRECTION;
  } else if (rightValid) {
    float error = RIGHT_TARGET_CM - rightCm;
    steering = KP * error;
    // if (fabsf(error) <= RIGHT_DEADBAND_CM) {
    //   steering = 0;
    // } else {
    //   steering = KP * error;
    // }
  } else {
    steering = 0;
  }
}