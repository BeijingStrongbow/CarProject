#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Servo.h>

Adafruit_MotorShield shield = Adafruit_MotorShield();
Servo s;
Adafruit_DCMotor *m = shield.getMotor(1);
int steering = 1; //0 = left, 1 = straight. 2 = right;
bool driving = false;

void setup() {
  shield.begin();
  s.attach(10);
  s.write(69);
  pinMode(0, INPUT); //forward
  pinMode(1, INPUT); //backward
  pinMode(2, INPUT); //left
  pinMode(3, INPUT); //right
}

void loop() {
  //FORWARD
  if(digitalRead(0) == 1 && !driving){
    accelerate(true);
  }
  
  //BACKWARD
  else if(digitalRead(1) == 1 && !driving){
    accelerate(false);
  }
  
  //LEFT
  else if(digitalRead(2) == 1 && !driving){
    steer(0);
    if(digitalRead(2) == 1){
      accelerate(true);
    }
    else{
      steer(1);
    }
  }
  
  //RIGHT
  else if(digitalRead(3) == 1 && !driving){
    steer(2);
    if(digitalRead(3) == 1){
      accelerate(true);
    }
    else{
      steer(1);
    }
  }

  else if(digitalRead(0) == 0 &&
          digitalRead(1) == 0 &&
          digitalRead(2) == 0 &&
          digitalRead(3) == 0 && 
          driving){
    decelerate();
    steer(1);
  }
}

void accelerate(bool forward){
  driving = true;
  for(int i = 0; i <= 255; i++){
    m->setSpeed(i);
    if(forward){
      m->run(FORWARD);
      if(digitalRead(0) == 0){
        driving = false;
        for(int j = i; j >= 0; j--){
          m->setSpeed(j);
        }
        break;
      }
    }
    else{
      m->run(BACKWARD);
      if(digitalRead(1) == 0){
        driving = false;
        for(int j = i; j >= 0; j--){
          m -> setSpeed(j);
        }
        break;
      }
    }
    delay(4);
  }
}

void decelerate(){
  for(int i = 254; i >= 0; i--){
    m->setSpeed(i); 
  }
  driving = false;
}

void steer(int direction){
  if(direction == 0 && steering != 0){
    s.write(43);
    steering = 0;
    if(steering == 1){
      delay(500);
    }
    else{
      delay(1000);
    }
  }
  else if(direction == 1 && steering != 1){
    s.write(69);
    steering = 1;
    delay(500);
  }
  else if(direction == 2 && steering != 2){
    s.write(95);
    steering = 2;
    if(steering == 1){
      delay(500);
    }
    else{
      delay(1000);
    }
  }
  
}
