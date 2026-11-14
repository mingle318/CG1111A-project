// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
const int RGBWait = 100; // In milliseconds (ms)
// Define time delay before taking another LDR reading
const int LDRWait = 50; // In milliseconds (ms)
// Pin Definitions
const byte  LDR = A3; // LDR sensor pin at A3
//const byte LED = 13; // Check Indicator to signal Calibration Completed
// Define colour sensor LED pins
int ledArray[] = {3,1,2};
// Placeholders for colour detected
int red = 0;
int green = 0;
int blue = 0;
// Floats to hold colour arrays
float colourArray[] = {0,0,0};
float whiteArray[] = {0,0,0};
float blackArray[] = {0,0,0};
float greyDiff[] = {0,0,0};
char colourStr[3][5] = {"R = ", "G = ", "B = "};
// 
typedef struct {
  int A_pin;
  int B_pin;
} Decoder;
Decoder myDecoder = {.A_pin = A0, .B_pin = A1};

void toggle_decoder_pin(int Y_pin) {
  //call to turn the output Y pin of the 2-to-4 decoder to 0V
  if (Y_pin == 0) {
    digitalWrite(myDecoder.A_pin, LOW);
    digitalWrite(myDecoder.B_pin, LOW);
  } else if (Y_pin == 1) {
    digitalWrite(myDecoder.A_pin, HIGH);
    digitalWrite(myDecoder.B_pin, LOW);
  } else if (Y_pin == 2) {
    digitalWrite(myDecoder.A_pin, LOW);
    digitalWrite(myDecoder.B_pin, HIGH);
  } else if (Y_pin == 3) {
    digitalWrite(myDecoder.A_pin, HIGH);
    digitalWrite(myDecoder.B_pin, HIGH);  
  }
}


void setBalance(){
  // Set white balance  
  Serial.println("Put White Sample For Calibration ...");
  delay(5000); // Delay for five seconds for getting sample ready
  //digitalWrite(LED,LOW); // Check Indicator OFF during Calibration
  // Scan the white sample.
  // Go through one colour at a time, set the maximum reading for each colour -- red, green and blue to the white array
  for(int i = 0; i<= 2; i++) {
    toggle_decoder_pin(ledArray[i]);
    delay(RGBWait);
    whiteArray[i] = getAvgReading(5); // Scan 5 times and return the average,
    toggle_decoder_pin(0);
    delay(RGBWait);
  }
  // Done scanning white, time for the black sample.
  // Set black balance
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000); // Delay for five seconds for getting sample ready
  // Go through one colour at a time, set the minimum reading for red, green and blue to the black array
  for(int i = 0; i <= 2; i++){
    toggle_decoder_pin(ledArray[i]);
    delay(RGBWait);
    blackArray[i] = getAvgReading(5);
    toggle_decoder_pin(0);
    delay(RGBWait);
    // The differnce between the maximum and the minimum gives the range
    greyDiff[i] = whiteArray[i] - blackArray[i];
  }
  // Delay another 5 seconds for getting ready colour objects
  Serial.println("Colour Sensor Is Ready.");
  Serial.print("whiteArray[]:");
  Serial.print(whiteArray[0]);
  Serial.print(", ");
  Serial.print(whiteArray[1]);
  Serial.print(", ");
  Serial.println(whiteArray[2]);

  Serial.print("blackArray[]:");
  Serial.print(blackArray[0]);
  Serial.print(", ");
  Serial.print(blackArray[1]);
  Serial.print(", ");
  Serial.println(blackArray[2]);

  Serial.print("greyDiff[]:");
  Serial.print(greyDiff[0]);
  Serial.print(", ");
  Serial.print(greyDiff[1]);
  Serial.print(", ");
  Serial.println(greyDiff[2]);

  delay(5000);
}

int getAvgReading(int times){
  // Find the average reading for the requested number of times of scanning LDR
  int reading;
  int total =0;
  // Take the reading as many times as requested and add them up
  for(int i = 0;i < times;i++){
    reading = analogRead(LDR);
    total = reading + total;
    delay(LDRWait);
  }
  // Calculate the average and return it
  return total/times;
}

void setup(){
// Set up the outputs for the colour sensor
  // for(int c = 0;c<=2;c++){
  //   pinMode(ledArray[c],OUTPUT);
  // }
  //pinMode(LED,OUTPUT); // Check Indicator -- OFF during Calibration
  pinMode(myDecoder.A_pin, OUTPUT);
  pinMode(myDecoder.B_pin, OUTPUT);
  digitalWrite(myDecoder.A_pin, LOW);
  digitalWrite(myDecoder.B_pin, LOW);
  Serial.begin(115200); // Begin serial communication
  setBalance(); // Calibration
  //digitalWrite(LED, HIGH); //Check Indicator -- ON after Calibration

}

void loop(){
  // Turn on one colour at a time and LDR reads 5 times
  delay(5000);
  for(int c = 0;c<=2;c++){
    Serial.print(colourStr[c]);
    toggle_decoder_pin(ledArray[c]); // Turn ON the LED, red, green or blue, one colour at a time.
    delay(RGBWait);
    // Get the average of 5 consecutive readings for the current colour and return an average
    colourArray[c] = getAvgReading(1);
    // The average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
    colourArray[c] = (colourArray[c] - blackArray[c])/(greyDiff[c])*255;
    toggle_decoder_pin(0); // Turn off the current LED colour
    delay(RGBWait);
    Serial.println(int(colourArray[c])); // Show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code
  } 
}
