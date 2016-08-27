#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Servo.h>

Adafruit_MotorShield shield = Adafruit_MotorShield();
Servo s;

int dcDirPin = 13;
int dcPwmPin = 11;
int forwardPin = 2;
int backwardPin = 4;
int leftPin = 6;
int rightPin = 9;

int driving = 0; //0 = stationary 1 = turning left 2 = forwards 3 = turning right 4 = backwards
void setup() {
  Serial.begin(9600);
  shield.begin();
  s.attach(10);
  s.write(69);
  pinMode(dcDirPin, OUTPUT);
  pinMode(dcPwmPin, OUTPUT);
  pinMode(forwardPin, INPUT);
  pinMode(backwardPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);

}

void loop() {
  Serial.print(digitalRead(forwardPin));
  
  if(digitalRead(forwardPin) == 1 && 
    digitalRead(backwardPin) == 0 && 
    digitalRead(leftPin) == 0 && 
    digitalRead(rightPin) == 0 && 
    driving != 2){
    
    if(driving != 0){
      decelerate(200);
    }
    
    drive(1, true);
    if(digitalRead(forwardPin) == 1){
      driving = 2;
      accelerate(220);
    }
  }
  else if(digitalRead(forwardPin) == 0 && 
    digitalRead(backwardPin) == 1 && 
    digitalRead(leftPin) == 0 && 
    digitalRead(rightPin) == 0 && 
    driving != 4){
    
    if(driving != 0){
      decelerate(200);
    }
    
    drive(1, false);
    if(digitalRead(backwardPin) == 1){
      driving = 4;
      accelerate(200);  
    }
  }
  else if(digitalRead(forwardPin) == 0 && 
    digitalRead(backwardPin) == 0 && 
    digitalRead(leftPin) == 1 && 
    digitalRead(rightPin) == 0 && 
    driving != 1){
    
    if(driving != 0){
      decelerate(200);
    }
    
    drive(0, true);
    if(digitalRead(leftPin) == 1){
      driving = 1;
      accelerate(220);  
    }
  }
  else if(digitalRead(forwardPin) == 0 && 
    digitalRead(backwardPin) == 0 && 
    digitalRead(leftPin) == 0 && 
    digitalRead(rightPin) == 1 && 
    driving != 3){
    
    if(driving != 0){
      decelerate(200);
    }
    
    drive(2, true);
    if(digitalRead(rightPin) == 1){
      driving = 3;
      accelerate(220);  
    }
  }
  if(digitalRead(forwardPin) == 0 && 
     digitalRead(backwardPin) == 0 &&
     digitalRead(leftPin) == 0 &&
     digitalRead(rightPin) == 0 && 
     driving != 0){
    driving = 0;
    decelerate(215);      
  }
}

bool drive(int turn, bool forward){
  if(turn == 0 && abs(s.read() - 40) > 3){
    s.write(40);

    delay(2000);
  }
  else if(turn == 1 && abs(s.read() - 71) > 3){
    s.write(69);

    delay(2000);
  }
  else if(turn == 2 && abs(s.read() - 95) > 3){
    s.write(95);

    delay(2000);
  }

  if(forward){
    digitalWrite(dcDirPin, LOW);
  }
  else{
    digitalWrite(dcDirPin, HIGH);
  }
}

void accelerate(int finalSpeed){
  for(int i = 0; i <= finalSpeed; i++){
    analogWrite(dcPwmPin, i);

    if(digitalRead(forwardPin) == 0 && 
     digitalRead(backwardPin) == 0 &&
     digitalRead(leftPin) == 0 &&
     digitalRead(rightPin) == 0){
      decelerate(i);
      break;
    }
    delay(15);
  }
}

void decelerate(int startSpeed){
  driving = false;
  for(int i = startSpeed - 1; i >= 0; i--){
    analogWrite(dcPwmPin, i);
    
    delay(3);
  }
}

