#include <AFMotor.h>      //add Adafruit Motor Shield library
#include <Servo.h>        // add servo library            
#include <NewPing.h>      //add Ultrasonic sensor library

#define TRIG_PIN A4 // Pin A0 on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PIN A5 // Pin A1 on the Motor Drive Shield soldered to the ultrasonic sensor
#define MAX_DISTANCE 300 // sets maximum useable sensor measuring distance to 300cm
#define MAX_SPEED 250// sets speed of DC traction motors to 150/250 or about 70% of full speed - to get power drain down.
#define MAX_SPEED_OFFSET 10 // this sets offset to allow for differences between the two DC traction motors
#define COLL_DIST 30// sets distance at which robot stops and reverses to 30cm
#define TURN_DIST COLL_DIST+20 // sets distance at which robot veers away from object
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.
Servo servo1;             // servo object
AF_DCMotor leftMotor1(1, MOTOR12_1KHZ); // create motor #1 using M1 output on Motor Drive Shield, set to 1kHz PWM frequency
AF_DCMotor rightMotor1(3, MOTOR34_1KHZ);// create motor #3, using M3 output, set to 1kHz PWM frequency

int curDist = 0;
int leftDist ,rightDist;
String motorSet = "";
int speedSet = 0;
int pos = 90;              // set position to 90 servo

//-------------------------------------------- SETUP LOOP ----------------------------------------------------------------------------
void setup(){
  servo1.attach(9);
  leftMotor1.setSpeed(200);
  leftMotor1.run(RELEASE);
  rightMotor1.setSpeed(200);
  rightMotor1.run(RELEASE);
  leftDist =0;
  rightDist = 0;
  motorSet = "FORWARD";
  leftMotor1.run(FORWARD);      // turn it on going forward
  rightMotor1.run(FORWARD);     // turn it on going forward
  leftMotor1.setSpeed(MAX_SPEED); 
  rightMotor1.setSpeed(MAX_SPEED); 
  delay(5000);
   
  /*for (speedSet = 100; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftMotor1.setSpeed(speedSet); 
    rightMotor1.setSpeed(speedSet); 
    delay(10);
  }*/
}

//---------------------------------------------MAIN LOOP ------------------------------------------------------------------------------
void loop() {
  curDist = readPing();   // read distance
  if (curDist < COLL_DIST) {check();}  // if forward is blocked change direction
  delay(100);
 }
//-------------------------------------------------------------------------------------------------------------------------------------
int readPing() { // read the ultrasonic sensor distance
  delay(200);   
  unsigned int uS = 0;
  while(uS == 0) {  // Sometimes a zero is returned - indicates a bad ping. Try again.
    //Serial.println("error - read zero");
    uS = sonar.ping_cm();
  }
  //int dist  = sonar.convert_cm(uS);
  return uS;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void check(){
    moveStop();
    curDist = readPing();
    for(pos = 90; pos<=170; ++ pos ){
      servo1.write(pos);
      delay(5); 
    }
    leftDist = readPing();
    delay(500);
    for(pos = 160; pos >=10; -- pos ){
      servo1.write(pos);
      delay(10); 
    }
    rightDist = readPing();
    delay(500);
    for(pos = 20; pos<=90; ++ pos ){
      servo1.write(pos);
      delay(5); 
    }
    if(leftDist < COLL_DIST && rightDist < COLL_DIST){
      backward();
      check();  
    } 
    if(leftDist > COLL_DIST && leftDist > rightDist){
      turnLeft();
      return;  
    } 
    if(rightDist > COLL_DIST && rightDist > leftDist){
      turnRight();  
      return;
    }
    
}
//------------------------------------------------------------------------------------------------------------------------------------
void backward(){
  motorSet = "BACKWARD";
  leftMotor1.run(BACKWARD);      // turn motor 1 forward
  rightMotor1.run(BACKWARD);
  delay(3000);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  motorSet = "RIGHT";
  leftMotor1.run(FORWARD);      // turn motor 1 forward
  rightMotor1.run(BACKWARD);// turn motor 3 backward
 // moveStop();
  //Speed();
  //rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
     
  delay(700); // run motors this way for 1500        
  forward();
        
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
  motorSet = "RIGHT";
  leftMotor1.run(BACKWARD);      // turn motor 1 forward
  rightMotor1.run(FORWARD);// turn motor 3 backward
 // moveStop();
  //Speed();
  //rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  delay(700); // run motors this way for 1500        
  forward();
        
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void forward(){
  motorSet = "FORWARD";
  leftMotor1.run(FORWARD);      // set both motors back to forward
  rightMotor1.run(FORWARD);
 }
//-------------------------------------------------------------------------------------------------------------------------------------
void moveStop() {
  leftMotor1.run(RELEASE);
  rightMotor1.run(RELEASE);
  delay(600); 
  }  // stop the motors.
//-------------------------------------------------------------------------------------------------------------------------------------
void Speed() {
   for (speedSet = 100; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftMotor1.setSpeed(speedSet); 
    rightMotor1.setSpeed(speedSet); 
    delay(10);
  }
 
}
//-------------------------------------------------------------------------------------------------------------------------------------
 
