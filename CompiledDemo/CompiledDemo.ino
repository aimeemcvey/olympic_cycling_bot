//Servos
#include <Servo.h> 
int th=200;
int RQTI=11;
int mRQTI=8;
int LQTI=7;
int mLQTI=9;
int mL;
int mR;
int R;
int L;
boolean Straight;
boolean inStraight;
boolean VeerLeft;
boolean VeerRight;
boolean outStraight;
boolean TurnRight;
boolean TurnLeft;
boolean Stop; 
boolean Pause;
boolean SquareLeft;
boolean SquareRight;
Servo servoLeft;
Servo servoRight;

//Comms
#define Rx 15 // DOUT to pin 15
#define Tx 14 // DIN to pin 14
int buttonState = 0;

//Accelerometer
const int xPin = 12;     // X output of the accelerometer
const int yPin = 13;     // Y output of the accelerometer

void setup() {
 //Servos
 Serial.begin(9600);
 servoLeft.attach(13); // attach pin 12 to left motor
 servoRight.attach(12); // attach pin13 to right motor
 //Comms
 Serial3.begin(9600); // type a char, then hit enter
 delay(500);
 pinMode(2, INPUT); // set up push button
 pinMode(3, OUTPUT); // set up green LED
 pinMode(4, OUTPUT); // set up red LED
 //Accelerometer
 // initialize the pins connected to the accelerometer as inputs:
 pinMode(xPin, INPUT);
 pinMode(yPin, INPUT);
 pinMode(10, OUTPUT);
}

void loop() {
  mL = rcTime(mLQTI); // calculate RC time for middle left QTI
  mR = rcTime(mRQTI); // calculate RC time for middle left QTI
  R  = rcTime(RQTI);  //calculate  RC time for middle left QTI
  L  = rcTime(LQTI);  //calculate  RC time for middle left QTI
inStraight=(mL>th && mR>th);
outStraight=(L<th && R<th);
Straight=(inStraight && outStraight);
VeerLeft=(outStraight && mL>th && mR<th);
VeerRight=(outStraight && mL<th && mR>th);
TurnRight=(R>th && mR<th && mL<th && L<th);
SquareRight=(R>th && mR>th && mL>th && L<th);
TurnLeft=(R<th && mR<th && mL<th && L>th);
SquareLeft=(R<th && mR>th && mL>th && L>th);
Pause=(R>th && L>th);
Stop=(R<th && L<th && mR<th && mL<th);

if(Pause){ //if outers are black, STOP
    servoRight.write(1500); 
    servoLeft.write(1500); 
    Serial.println("Pause");
    delay(1000);
    servoRight.write(1550);
    servoLeft.write(1450);
    delay(500); 
  }
 if(Stop){
    servoRight.write(1450); 
    servoLeft.write(1550); 
    Serial.println("Stop");
  }
  else if(VeerRight){ //need to veer left 
    servoRight.write(1550);
    servoLeft.write(1490); 
    Serial.println("veer right");
  }
  else if(VeerLeft){ //need to veer right
    servoRight.write(1510);
    servoLeft.write(1450); 
    Serial.println("veer left");
  }
  
  else if(Straight){ //need to go straight
    servoRight.write(1600);
    servoLeft.write(1400); 
    Serial.println("straight");
  }
  else if(TurnRight || SquareRight){ // need to turn right
    servoRight.write(1520);
    servoLeft.write(1520);
   Serial.println("turn left");
  }
  else if(TurnLeft || SquareLeft){ //need to turn left
    servoRight.write(1480);
    servoLeft.write(1480);
    Serial.println("turn right");
  }
}

long rcTime(int pin) {
 pinMode(pin, OUTPUT);
 digitalWrite(pin, HIGH);
 delayMicroseconds(12000);
 pinMode(pin, INPUT);
 digitalWrite(pin, LOW); 
 long time = micros(); 
 while(digitalRead(pin));{ 
 time =micros() - time; 
 pinMode(pin,OUTPUT);
 digitalWrite(pin, LOW);
 }
 return time;
} 
