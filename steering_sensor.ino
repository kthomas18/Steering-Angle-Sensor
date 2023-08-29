/* Takes in input from multi-turn potentiometer. File that saves to CSV is
 *  write_to_csv Processing file.
 *  
 *  First few seconds is calibration, during which steering wheel should be turned all the way left and all the way right.
 *  Note that due to gear setup, the direction of turn angle is flipped from normal
 *  CCW positive and CW negative (interpret negative as left and positive as right).
 *  
 *  
 *  Based on example sketches: Calibration and Analog I/O
 */

// Constants
const int SENSOR_PIN = A0;    // select the input pin for the potentiometer
const int LED_PIN = 13;      // select the pin for the LED
const int BAUD_RATE = 9600;
const int STEER_ANGLE_MAX = 180; // Set to minimum of steering angle range
const int STEER_ANGLE_MIN = -180; // Set to maximum of steering angle range

// Variables 
int sensorValue = 0;         // the sensor value
int sensorMin = 0;        // minimum sensor value
int sensorMax = 1023;           // maximum sensor value

// Calibrates the sensor during the first ten seconds so that 
// range of steering wheel is mapped to range of sensor.
void calibrate() {
  digitalWrite(LED_PIN, HIGH);

  // calibrate during the first ten seconds
  while (millis() < 10000) {
    sensorValue = analogRead(SENSOR_PIN);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }

  // signal the end of the calibration period
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  // declare the LED_PIN as an OUTPUT:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(BAUD_RATE);

  calibrate();

}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(SENSOR_PIN);

  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, sensorMin, sensorMax, STEER_ANGLE_MIN, STEER_ANGLE_MAX);

  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, STEER_ANGLE_MIN, STEER_ANGLE_MAX);

  // fade the LED using the calibrated value:
  analogWrite(LED_PIN, sensorValue);
  
  Serial.println(sensorValue);
}
