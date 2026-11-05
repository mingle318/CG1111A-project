mBot wall control - multiple Arduino tabs

1. Extract the ZIP. Keep all files together in the mbot_wall_tabs folder.
2. Open mbot_wall_tabs.ino in Arduino IDE. Other source files appear as tabs.
3. Use the Makeblock library required by the original sketch; select the
   original mBot/mCore target (Arduino Uno) and its USB port.
4. Edit Config.h for sensor wiring, IR calibration, motor powers and timings.
5. Click Verify, then Upload. This uploads ONE combined program.

Files
mbot_wall_tabs.ino : shared robot state, setup() and loop()
Config.h           : all user settings, IR calibration table and action choice
Functions.h        : function declarations; usually leave this alone
Motors.ino         : motor output and straight driving
Sensors.ino        : IR, ultrasonic, calibration output and black-line reads
WallControl.ino    : wall validation state reset and steering calculation
Maneuvers.ino      : turn plans, U-turns and their execution

There must be only one setup() and one loop() in the whole sketch.
Do not add the old complete sketch into this folder: that duplicates functions.
Do not #include the other .ino files; Arduino combines them automatically.
The main .ino filename must match its folder name.

Calibration
The IR model is still unknown. This version assumes an analog ranging sensor,
not a digital on/off detector. Ultrasonic is assumed on the left, port 3 S2
(A3); IR output on the right, port 4 S2 (A1); line follower on port 2.
Use the IR sensor's specified supply and common ground.

Until every -1 in IR_ADC is replaced with valid calibration readings, the
motors remain stopped and Serial Monitor at 115200 baud displays measurements.
Measure raw IR values at the distances in IR_CM; change these distances to
stay within your sensor's valid, monotonic measurement range. Enter the
readings in Config.h and upload again. Close-range ambiguity in some IR
sensors cannot be resolved by this table alone.

After calibration, startup is automatic after one second. Start on white.
Turns and one-grid travel remain timed and require physical calibration.
Black lines are ignored throughout an entire maneuver; wall correction is
allowed during the U-turn's straight leg. No-wall travel uses base powers.
Colour sensing is not implemented.

Validation: desktop C++ simulation only, not physical robot or AVR testing.
