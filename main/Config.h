#pragma once

const int DRIVE_LEFT = 255; //215
const int DRIVE_RIGHT = 255; //230
const int TURN_LEFT = 220;
const int TURN_RIGHT = 255;

const uint32_t LEFT_90_MS = 300;
const uint32_t RIGHT_90_MS = 320;
const uint32_t TURN_180_MS_LEFT = 600;
const uint32_t TURN_180_MS_RIGHT = 600;
const uint32_t GRID_MS_RIGHT = 650;
const uint32_t GRID_MS_LEFT = 650;


const uint32_t START_DELAY_MS = 1000;
const uint32_t SETTLE_MS = 200;
const uint32_t MAX_RUN_MS = 0; //set to 0 to disable. For debugging purpose.

const int LEFT_FORWARD_SIGN = -1;
const int RIGHT_FORWARD_SIGN = 1;

const float SPEED_OF_SOUND = 0.0343; //cm/us

enum Action {
  STOP_AT_LINE, LEFT_90, RIGHT_90, TURN_180, U_LEFT, U_RIGHT, STOP
};

const Action ACTION[] = {LEFT_90, U_RIGHT, RIGHT_90, TURN_180, U_LEFT, STOP};

const int RGB_WAIT = 100;
const int LDR_WAIT = 50;
const int IR_WAIT = 50; // In microseconds (us)
// Pin Definitions
const int LDR_PIN = A3;
// const int IR_EMITTER_PIN = A0;
const uint8_t US_PIN = 12;
const uint8_t IR_PIN = A2;

const int LED_ARRAY[] = {3, 1, 2};

const float WHITE_ARRAY[] = {988.00, 901.00, 920.00};
const float BLACK_ARRAY[] = {899.00, 507.00, 598.00};
const float GREY_DIFF[] = {89.00, 394.00, 322.00};

const char COLOUR_STR[3][5] = {"R = ", "G = ", "B = "}; //For debugging
const String COLOR_TAG[6] = {"Red", "Blue", "Green", "Orange", "Pink", "White"};

typedef struct {
  int A_pin;
  int B_pin;
} Decoder;
const Decoder DECODER = {.A_pin = A0, .B_pin = A1};



const int IR_ARRAY[10] = {-926, -511, -360, -291, -240, -220, -205, -197, -190, -183};


const int COLOUR_TARGET[7][3] = {
  {255, 0, 0}, //Red
  {0, 0, 255}, //Blue
  {0, 255, 0}, //Green
  {255, 90, }, //Orange
  {255, 105, 180}, //Pink
  {255, 255, 255}, //White
  {0, 0, 0} //Black
};


const float IR_TOO_CLOSE = -160; //IR raw value
const float RIGHT_MIN_CM = 9.0;
const float RIGHT_MAX_CM = 13.5;
const float RIGHT_DEADBAND_CM = 2.25f;
const float RIGHT_TARGET_CM = 11.0;
const float RIGHT_WALL_LIMIT_CM = 15;
const float LEFT_CORRECTION = 180;
const float KP = 12.0f;
const uint32_t US_TIMEOUT_US = 1500;
const uint32_t WALL_SAMPLE_MS = 30;

