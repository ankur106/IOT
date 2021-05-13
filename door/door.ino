
#include <Servo.h>        // add servo library            
#include <NewPing.h>      //add Ultrasonic sensor library

#define TRIG_PIN 12// Pin A0 on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PIN 13   // Pin A1 on the Motor Drive Shield soldered to the ultrasonic sensor
#define MAX_DISTANCE 300 // sets maximum useable sensor measuring distance to 300cm
#define SERVO_PIN 7

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.
Servo Servo1;

int curDist = 0;



void setup() {
  // put your setup code here, to run once:
  Servo1.attach(SERVO_PIN);
  Servo1.write(0);
  Serial.begin(9600);
  //Serial.setTimeout(10000);
}

void loop() {
  curDist = 0;
  
  
  
  curDist = readPing();
  //Serial.println("in loop");
  Serial.println(curDist);
  
  if(curDist<50){
    Serial.println("Enter Password:  ");
    
    
    String str = "";
    Serial.flush();

    int counter = 0;
    int wait_time = 5;
    while(( str = Serial.readString()) == "" && counter<wait_time ){
      counter++;
      Serial.print(counter);
      delay(1000);  
    }
      Serial.println("");
    if(counter<wait_time){
      str.trim();
      Serial.println(str);
      Serial.flush();

      if(str.equals("Ankur")){
        Serial.println("WELCOME");
        Serial.println("Door is Open");
        
        Servo1.write(90);
        delay(2500);
        while((curDist=readPing())<50){

          delay(2000);
          //Serial.print(curDist);
        }
        Serial.println("Door will be closed in 5 seconds.");
        delay(5000);
        
      }
      else{
        Serial.println("Wrong Password");
      }
      
    }
    
    
    
  }
  Servo1.write(0);
  delay(1000);
  
  
  

}


int readPing() { // read the ultrasonic sensor distance
  delay(200);   
  unsigned int uS = 0;
  while(uS == 0) {  // Sometimes a zero is returned - indicates a bad ping. Try again.
    //Serial.println("error - read zero");
    //Serial.println("in read ping");
    uS = sonar.ping_cm();
  }
  //int dist  = sonar.convert_cm(uS);
  return uS;
}
