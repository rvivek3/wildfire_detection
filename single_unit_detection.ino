//Arduino Wildfire Detection Code
//eCYBERMISSION 2015
//Coded by Rajan Vivek
//This program is designed to monitor the environment to perform early detection of wildfires
//This program represents one unit, although a grid is planned for monitoring of a large region
//Readings are currently sped up to display functionality:
//Readings are taken every five seconds normally, every four seconds when smoke is detected,
every 2 seconds when fire is detected
// The Connections
int flameSensorNorth = A2; //analog sensor
int flameSensorEast = A1; //analog sensor
int tempSensor = A5; //analog sensor
int smokeSensor = 12; //digital sensor
int redLED = 6; //digital output
int greenLED = 7; //digital output
// Establish Variables for Inputs
int flameNorthReading = 0;
int flameEastReading = 0;
int tempReading = 0;
int smokeReading = 0;
//Placeholders
int ambientLightNorth = 0;
int ambientLightEast = 0;
long currentTime = 0;
//These variables allow the unit to know whether certain conditions ever existed in the recent
past
int fireProcedureCycle = 0;
int fireCycleNorth = 0;
int fireCycleEast = 0;
int smokeCycle = 0;
void setup() {
Serial.begin(9600);
pinMode(redLED, OUTPUT); //This will be initiated when it is night or if there is a fire
pinMode(greenLED, OUTPUT); //This will be initiated when the unit is currently taking readings
pinMode(smokeSensor, INPUT);
flameNorthReading = analogRead(flameSensorNorth);
flameEastReading = analogRead(flameSensorEast);
tempReading = analogRead(tempSensor);
smokeReading = digitalRead(smokeSensor);
// Calibrate Flame Sensors for five seconds
while (millis() < 5000) {
ambientLightNorth = analogRead(flameSensorNorth);

ambientLightEast = analogRead(flameSensorEast);
}
}
void loop() {
digitalWrite(greenLED, HIGH);
String unitCoordinates = "(1,1)";
currentTime = millis()/1000;
//Basic Information Printed
Serial.print("Time (seconds since activation): "); Serial.println(currentTime);
Serial.print("Unit Coordinates: "); Serial.println(unitCoordinates);
//Check for smoke
smokeReading == 0;
smokeReading = digitalRead(smokeSensor);
if(smokeReading == 1) {
Serial.println("Smoke Detected in Region");
smokeCycle = 1;
}
else {
if(smokeCycle > 0) {
Serial.println("Smoke is no longer detected in the region");
smokeCycle = 0;
}
}
//Check for Fire
// Ambient light is removed for accuracy
// Lower the reading --> closer the flame to sensor
flameNorthReading = analogRead(flameSensorNorth) - ambientLightNorth;
flameEastReading = analogRead(flameSensorEast) - ambientLightEast;
if(flameNorthReading < -300 || flameEastReading < -300) {
digitalWrite(redLED, HIGH);
digitalWrite(greenLED, HIGH);
if(flameNorthReading < -300) {
fireCycleNorth = 1;
if(flameNorthReading < -600) {
Serial.print("Close-Range Fire in Northern Quadrant! Approximate Coordinates: (1,2)");
Serial.println("\n");
}
else {
Serial.print("Distant Fire in Northern Quadrant! Approximate Coordinates: (1,3)");
Serial.println("\n");
}
}
if(flameEastReading < -300) {
fireCycleEast = 1;
if(flameEastReading < -600) {

Serial.print("Close-Range Fire in Eastern Quadrant! Approximate Coordinates: (2,1)");
Serial.println("\n");
}
else {
Serial.print("Distant Fire in Eastern Quadrant! Approximate Coordinates: (3,1)");
Serial.println("\n");
}
}
//alert if fire becomes absent in one quadrant while still existing in other
if(fireProcedureCycle > 0) {
if(fireCycleNorth > 0 && fireCycleEast > 0) {
if(flameNorthReading > -300) {
Serial.print("Fire is no longer detected in the Northern Quadrant");
Serial.println("\n");
fireCycleNorth = 0;
}
if(flameEastReading > -300) {
Serial.print("Fire is no longer detected in the Eastern Quadrant");
Serial.println("\n");
fireCycleEast = 0;
}
}
}
delay(1000);
digitalWrite(greenLED, LOW);
delay(1000);
fireProcedureCycle +=1;
//Separate readings
Serial.println("------------------------------------------------------------------------");
}
else {
//Concluding stats after fire is terminated
if(fireProcedureCycle > 0) {
Serial.print("No fire detected in range of unit, ");
int fireDuration = fireProcedureCycle * 2;
Serial.print("Duration of Fire (seconds): ");
Serial.println(fireDuration);
fireProcedureCycle = 0;
digitalWrite(redLED, LOW);
}
//Reset Ambient Light
ambientLightNorth = analogRead(flameSensorNorth);
ambientLightEast = analogRead(flameSensorEast);
//Check if Night to initiate red light
if(ambientLightNorth > 1000 && ambientLightEast > 1000) {
digitalWrite(redLED, HIGH);
}
else {

digitalWrite(redLED, LOW);
}
//Print Temperature Every 5 seconds
//Convert sensor reading to millivolts to degrees Celsius and Fahrenheit
digitalWrite(greenLED, HIGH);
tempReading = analogRead(tempSensor);
tempReading = analogRead(tempSensor);
float mV = (tempReading * (5000.0/1024.0));
float tempC = ((mV -500.0)/10.0);
Serial.print("Current Temperature (C): ");
Serial.println(tempC);
float tempF = (tempC * (9.0/5.0) + 32.0);
Serial.print("Current Temperature (F): ");
Serial.println(tempF);
Serial.println("------------------------------------------------------------------------");
Serial.print("\n"); //skip line
delay(1000);
digitalWrite(greenLED, LOW);
//Check stats more frequently (every 4 seconds) if there is smoke
smokeReading = digitalRead(smokeSensor);
if(smokeReading == 1) {
delay(3000);
}
else {
delay(4000);
}
}
}
