
int getAverageIr() {
  double total = 0;
  for(int i = 0; i < 5; i++) {
    delay(IR_WAIT);
    total += analogRead(IR_PIN);
  }
  return (int)total / 5;
}

int readIrRaw() {
  // Discard the first ADC conversion after a possible channel change.
  toggleDecoderPin(1);
  // digitalWrite(IR_EMITTER_PIN, HIGH);
  int offset = getAverageIr();
  // Serial.print("OFFSET: ");
  // Serial.println(offset);
  toggleDecoderPin(0);
  // digitalWrite(IR_EMITTER_PIN, LOW);
  return getAverageIr() - offset;
}

double getIRDist() {
  //IR emitter should be turned on before calling this
  
  int read = readIrRaw();
  if (read < IR_ARRAY[0]) {
    return 0.0;
  }
  if (read > IR_ARRAY[9]) {
    return 11.0;
  }//clip out of range values, else:

  int i = 0;
  for(i = 0; i < 9; i++) {
    if(read <= IR_ARRAY[i+1]) {
      break;
    }// else continue
  }//exit: IR_range_arr[i] <= read <= IR_range_arr[i+1]

  int range = IR_ARRAY[i+1] - IR_ARRAY[i];
  double dist = i + 1 + ((double)(read - IR_ARRAY[i]) / range);
  //plus 1 because 0 indexed
  Serial.print(F("IR_RAW="));
  Serial.print(read);
  Serial.print("  IR_cm=");
  Serial.print(dist);
  return dist;
}


float ultrasonicDistanceCm() {
  pinMode(US_PIN, OUTPUT);
  digitalWrite(US_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(US_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_PIN, LOW);
  pinMode(US_PIN, INPUT);
  unsigned long echoUs = pulseIn(US_PIN, HIGH, US_TIMEOUT_US);
  if (!echoUs) return -1;
  float cm = echoUs * SPEED_OF_SOUND * 0.5;
  return cm;
}

void printCalibration() {
  uint32_t now = millis();
  if ((uint32_t)(now - lastPrintAt) < 250) return;
  lastPrintAt = now;
  float us = ultrasonicDistanceCm();
  // double ir_cm = getIRDist();
  int ir_raw = readIrRaw();
  Serial.print("IR_raw= ");
  Serial.print(ir_raw);
  Serial.print(F("  ultrasonic_cm="));
  Serial.println(us, 1);
  // Serial.print("Steering: ");
  // Serial.println(steering);
}

bool blackDetected() {
  return lineSensor.readSensors() != S1_OUT_S2_OUT;
}

void toggleDecoderPin(int Y_pin) {
  //call to turn the output Y pin of the 2-to-4 decoder to 0V
  if (Y_pin == 0) {
    digitalWrite(DECODER.A_pin, LOW);
    digitalWrite(DECODER.B_pin, LOW);
  } else if (Y_pin == 1) {
    digitalWrite(DECODER.A_pin, HIGH);
    digitalWrite(DECODER.B_pin, LOW);
  } else if (Y_pin == 2) {
    digitalWrite(DECODER.A_pin, LOW);
    digitalWrite(DECODER.B_pin, HIGH);
  } else if (Y_pin == 3) {
    digitalWrite(DECODER.A_pin, HIGH);
    digitalWrite(DECODER.B_pin, HIGH);  
  }
}

int getAvgReading(int times) {
  // Find the average reading for the requested number of times of scanning LDR
  int reading;
  int total =0;
  // Take the reading as many times as requested and add them up
  for(int i = 0;i < times;i++){
    reading = analogRead(LDR_PIN);
    total = reading + total;
    delay(LDR_WAIT);
  }
  // Calculate the average and return it
  return total/times;
}



int getClosestColour() {
  double input[3] = {0, 0, 0};
  Serial.print("{");
  for(int c = 0; c <= 2; c++){
    // Serial.print(COLOUR_STR[c]);
    toggleDecoderPin(LED_ARRAY[c]); // Turn ON the LED, red, green or blue, one colour at a time.
    delay(RGB_WAIT);
    // Get the average of 5 consecutive readings for the current colour and return an average
    input[c] = getAvgReading(5);
    // The average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
    input[c] = (input[c] - BLACK_ARRAY[c])/(GREY_DIFF[c])*255;
    toggleDecoderPin(0); // Turn off the current LED colour
    delay(RGB_WAIT);
    Serial.print(int(input[c]));
    if(c < 2)
    Serial.print(", ");
  } 
  Serial.println("}");
  
  int output = -1;
  double min_dist_to_target = 0;
  double cur_dist_to_target = 0;

  for(int i = 0; i < 6; i++) {
    cur_dist_to_target = 0;
    //sum the square of differences
    for(int j = 0; j < 3; j++) {
      double sq_component = pow((input[j] - COLOUR_TARGET[i][j]), 2);
      cur_dist_to_target += sq_component;
    }
    //compare the distances, keep the smallest one
    if(i == 0) {
      //initialise first guess
      min_dist_to_target = cur_dist_to_target;
      output = i;
    } else if(cur_dist_to_target < min_dist_to_target) {
      min_dist_to_target = cur_dist_to_target;
      output = i;
    }
  }
  return output;
}

int guessColorByDistance() {
  int cnt[6] = {0, 0, 0, 0, 0, 0};
  for(int i = 1; i <= 5; i++) { //Guess 5 times
    cnt[getClosestColour()]++;
  }
  int color = 0;
  for(int i = 1; i < 6; i++) {
    if(cnt[i] > cnt[color]) color = i;
  }
  return color;
}

int guessColorByValue() {
  double input[3] = {0, 0, 0};
  Serial.print("{");
  for(int c = 0; c <= 2; c++){
    toggleDecoderPin(LED_ARRAY[c]);
    delay(RGB_WAIT);
    input[c] = getAvgReading(1);
    input[c] = (input[c] - BLACK_ARRAY[c])/(GREY_DIFF[c])*255;
    toggleDecoderPin(0);
    Serial.print(int(input[c]));
    if(c < 2)
    Serial.print(", ");
  } 
  Serial.println(int(input[0]));
  Serial.println("}");
  if(input[1] >= 240 && input[2] >= 240) {
    return 5; //White
  }
  if(input[1] < 150 && input[2] < 150) {
    return 0; //Red
  }
  if(input[2] < 120) {
    return 3; //Orange
  }
  if(input[0] < 200) {
    if(input[2] > input[1]) return 1; //Blue;
    else return 2; //Green
  }
  
  return 4; //Pink
}

