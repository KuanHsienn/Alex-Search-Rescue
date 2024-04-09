

#include <string.h>

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 32 // PC5
#define S1 31 // PC6
#define S2 33 // PC4
#define S3 34 // PC3
#define sensorOut 41 // PG0

// LED pins
#define blueLED 46 // PL3
#define redLED 45 // PL4
#define greenLED 44 // PL5

#define ECHO 20 // yellow // PD1 // PA2
#define TRIG 21 // orange // PD0 // PA3

// Calibration Values - 3cm between sensor and color paper
// *Get these from Calibration Sketch
int redMin = 210; //190; // Red minimum value
int redMax = 600; //810; // Red maximum value
int greenMin = 110; //185; // Green minimum value
int greenMax = 720; //800; // Green maximum value
int blueMin = 90; //155; // Blue minimum value
int blueMax = 610; //644; // Blue maximum value



// Function to read Red Pulse Widths
int getRedPW() {
	// Set sensor to read Red only
  PORTC &= 0b11100111;
	//digitalWrite(S2,LOW);
	//digitalWrite(S3,LOW);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

// Function to read Green Pulse Widths
int getGreenPW() {
	// Set sensor to read Green only
  PORTC |= 0b00011000;
	//digitalWrite(S2,HIGH);
	//digitalWrite(S3,HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

// Function to read Blue Pulse Widths
int getBluePW() {
	// Set sensor to read Blue only
	PORTC &= 0b11101111;
  PORTC |= 0b00001000;
  //digitalWrite(S2,LOW);
	//digitalWrite(S3,HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

void checkColour(int red, int green, int blue) {
  double sum = (double)(red+green+blue);
  double rRatio = fabs(red/sum);
  double gRatio = fabs(green/sum);
  double bRatio = fabs(blue/sum);
  Serial.print("RedRatio = ");
	Serial.print(rRatio);
	Serial.print(" - GreenRatio = ");
	Serial.print(gRatio);
	Serial.print(" - BlueRatio = ");
	Serial.println(bRatio);
  if (rRatio > 0.28 && gRatio > 0.28 && bRatio > 0.28) {
    // white
    Serial.println("white");
  }
  else if (rRatio > gRatio && rRatio > bRatio) {
    // red
    Serial.println("red");
  }
  //else if (rRatio > 0.45 && gRatio < 0.3) {
    // orange
  //  Serial.println("orange");} 
  else if (gRatio > rRatio && gRatio > bRatio) {
    // green
    Serial.println("green");
  }
  //else if ((rRatio >= 0.4 && gRatio > 0.20 && bRatio > 0.35) || (gRatio < 0.3 && bRatio > 0.3)) {
    // purple
  //  Serial.println("purple");}
  //else if (rRatio < 0.4 && bRatio >= 0.35) {
    // blue
  //  Serial.println("blue");}
}

float getDistance() {
  PORTD |= 0b00000001;
  //digitalWrite(TRIG, HIGH);
  delay(100);
  PORTD &= 0b11111110;
  //digitalWrite(TRIG, LOW);
  float microsecs = pulseIn(ECHO, HIGH);
  float cms = microsecs * 0.0345 / 2;
  return cms; 
}

void colourSense() {
  // Read Red value
	redPW = getRedPW();
	// Map to value from 0-255
	redValue = map(redPW, redMin,redMax,255,0);
	// Delay to stabilize sensor
	delay(200);

	// Read Green value
	greenPW = getGreenPW();
	// Map to value from 0-255
	greenValue = map(greenPW, greenMin,greenMax,255,0);
	// Delay to stabilize sensor
	delay(200);

	// Read Blue value
	bluePW = getBluePW();
	// Map to value from 0-255
	blueValue = map(bluePW, blueMin,blueMax,255,0);
	// Delay to stabilize sensor
	delay(200);

  Serial.print("RedPWM = ");
	Serial.print(redPW);
	Serial.print(" - GreenPWM = ");
	Serial.print(greenPW);
	Serial.print(" - BluePWM = ");
	Serial.println(bluePW);

	// Print output to Serial Monitor
	Serial.print("Red = ");
	Serial.print(redValue);
	Serial.print(" - Green = ");
	Serial.print(greenValue);
	Serial.print(" - Blue = ");
	Serial.println(blueValue);

  // Show colour on LED based on colour detected by colour sensor
  analogWrite(redLED, abs(redValue));
  analogWrite(greenLED, abs(greenValue));
  analogWrite(blueLED, abs(blueValue));
  delay(500);

  // Turn off LED
  analogWrite(greenLED, 0);
  analogWrite(blueLED, 0);
  analogWrite(redLED, 0);

  checkColour(redValue, greenValue, blueValue);
}
