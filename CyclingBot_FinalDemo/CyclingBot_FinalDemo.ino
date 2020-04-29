//Xbee
#define Rx 15 // DOUT to pin 15
#define Tx 14 // DIN to pin 14

//Servos
#include <Servo.h>
Servo servoLeft;
Servo servoRight;
int th=300;
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
int counter=0;

//Accelerometer
const int xPin = 2;     // X output of the accelerometer
const int yPin = 3;     // Y output of the accelerometer

void setup() {
   Serial.begin(9600);
   Serial3.begin(9600); // begin comms
   Serial2.begin(9600); // LCD screen
   Serial2.write(12); // begin LCD
   delay(500);
   servoLeft.attach(13); // attach pin 12 to left motor
   servoRight.attach(12); // attach pin13 to right motor
    // initialize the pins connected to the accelerometer as inputs:
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
    pinMode(10, OUTPUT); // initialize back red LED
    pinMode(11, OUTPUT);
    pinMode(4,OUTPUT); // initialize front red LED
    pinMode(5,OUTPUT); // initialize green LED
    pinMode(6,OUTPUT); // initialize yellow LED
}

void loop() {
  circle();

}

void circle()
{
  mL = rcTime(mLQTI); // calculate RC time for middle left QTI
  mR = rcTime(mRQTI); // calculate RC time for middle left QTI
  R  = rcTime(RQTI);  // calculate  RC time for middle left QTI
  L  = rcTime(LQTI);  // calculate  RC time for middle left QTI
inStraight=(mL>th && mR>th); // set up conditions for line following
outStraight=(L<th && R<th); // less than threshold is white, greater is black
Straight=(inStraight && outStraight);
VeerLeft=(outStraight && mL>th && mR<th);
VeerRight=(outStraight && mL<th && mR>th);
TurnRight=(R>th && mR<th && mL<th && L<th);
SquareRight=(R>th && mR>th && mL>th && L<th);
TurnLeft=(R<th && mR<th && mL<th && L>th);
SquareLeft=(R<th && mR>th && mL>th && L>th);
Pause=(R>th && L>th && mR>th && mL>th);
Stop=(R<th && L<th && mR<th && mL<th);

Serial.println(counter); // when counter is <1, follow circle
if(Pause && counter<1){ // if outers are black, reached hash, turn in
    servoRight.write(1550); // forward
    servoLeft.write(1450);
    delay(300);
    servoRight.write(1500); // pause
    servoLeft.write(1500); 
    delay(250);
    servoRight.write(1460); // turn left into circle
    servoLeft.write(1460);
    delay(1400);
    Serial.println("hash");
    counter++; // increase counter
    Serial.println(counter);
    servoRight.write(1550); // forward
    servoLeft.write(1450);
    delay(300);
  }
  else if(Pause && counter>0){ // bot has reached ramp
    servoRight.write(1550); // forward
    servoLeft.write(1450); 
    delay(250);
    Serial.println("hash2");
    ramp(); // enter ramp loop
  }
 if(Stop){ // if read all white, back up to find line again
    servoRight.write(1450);
    servoLeft.write(1550); 
    Serial.println("reverse");
  }
  else if(VeerRight){ // need to veer left 
    servoRight.write(1550);
    servoLeft.write(1490); 
    Serial.println("veer right");
  }
  else if(VeerLeft){ // need to veer right
    servoRight.write(1510);
    servoLeft.write(1450); 
    Serial.println("veer left");
  }
  else if(Straight){ // need to go straight
    servoRight.write(1600);
    servoLeft.write(1400); 
    Serial.println("straight");
  }
  else if(R>th){ // need to turn right
    servoRight.write(1520);
    servoLeft.write(1520);
   Serial.println("turn left");
  }
  else if(L>th){ //need to turn left
    servoRight.write(1480);
    servoLeft.write(1480);
    Serial.println("turn right");
  }
}
long rcTime(int pin) { 
 pinMode(pin, OUTPUT); // set up pin for output
 digitalWrite(pin, HIGH); // turn on and delay for 0.012 s to charge
 delayMicroseconds(12000);
 pinMode(pin, INPUT); // change pin to input
 digitalWrite(pin, LOW); // turn off
 long time = micros(); 
 while(digitalRead(pin));{ // track time to discharge
 time = micros() - time; 
 pinMode(pin,OUTPUT); // change pin back to output
 digitalWrite(pin, LOW);
 }
 return time;
} 

void ramp(){ // bot needs to travel up ramp
double time0=0;
  time0=micros();                                                                           
 // drive up the ramp
 while((micros()-time0)<800000){
   servoRight.write(1550);
   servoLeft.write(1450);
 }
   // variables to read the pulse widths:
  int pulseX, pulseY;
  // variables to contain the resulting accelerations
  int accelerationX, accelerationY;
  // read pulse from x- and y-axes:
  pulseX = pulseIn(xPin, HIGH);
  pulseY = pulseIn(yPin, HIGH);
  // convert the pulse width into acceleration
  // accelerationX and accelerationY are in milli-g's:
  // Earth's gravity is 1000 milli-g's, or 1 g.
  accelerationX = ((pulseX / 10) - 500) * 8;
  accelerationY = ((pulseY / 10) - 500) * 8;
  Serial.println(accelerationX);

  if(accelerationX>110){ // accelerometer reads biggest ramp
    digitalWrite(10,HIGH); // flash back red LED twice
    delay(500);
    digitalWrite(10,LOW);
    delay(500);
    digitalWrite(10,HIGH);
    delay(500);
    digitalWrite(10,LOW);
    Serial.println("big");
    Serial3.print("g"); // trasmits g for 2
    delay(500);
    Serial.println("big");
    servoRight.write(1500); // bot stops
    servoLeft.write(1500);
    delay(1000);
    servoLeft.detach(); // detach pin 12 from left motor
    servoRight.detach(); // detach pin 13 from right motor
    receive(); // jump to receive comms
  }
  else if(accelerationX>30 && accelerationX<110){ // accelerometer reads medium ramp
    digitalWrite(10,HIGH); // flash back red LED once
    delay(500);
    digitalWrite(10,LOW);
    Serial.println("medium");
    Serial3.print("h"); // trasmits h for 1
    Serial.println("medium");
    delay(500);
    servoRight.write(1500); // bot stops
    servoLeft.write(1500);
    delay(1000);
    servoLeft.detach(); // detach pin 12 from left motor
    servoRight.detach(); // detach pin 13 from right motor
    receive(); // jump to receive comms
  }
  else{ // accelerometer reads threshold less than medium ramp
    Serial.println("0");
    Serial3.print("i"); // trasmits i for 0
    delay(500);
    servoRight.write(1500); // bot stops
    servoLeft.write(1500);
    delay(1000);
    servoLeft.detach(); // detach pin 12 from left motor
    servoRight.detach(); // detach pin 13 from right motor
    receive(); // jump to receive comms
    }
}

void receive(){ // recieve comms from team sentry
    boolean sad=false;
    char incoming;
    while(sad==false){ // initialize while loop for team receiving
      if(Serial3.available()){ // waits to receive comms
        incoming = Serial3.read();
        Serial.println(incoming);
        if(incoming=='1' || incoming=='2'||incoming=='0'){ // until receives 0, 1, 2
          sad=true; // exits while loop, enters corresponding if tree for score
          Serial.println(incoming);
        }
      }
    }
      Serial.println(incoming); // prints incoming message on monitor
      if(incoming=='2'){ // if gold medal, blinks 2 times
      Serial2.print('2'); // print team score on LCD
      digitalWrite(4, HIGH); // front red LED turns on
      delay(500); // remains lit for 500 ms
      digitalWrite(4, LOW); // front red LED turns off
      delay(500);
      digitalWrite(4, HIGH); // front red LED turns on
      delay(500); // remains lit for 500 ms
      digitalWrite(4, LOW); // back red LED turns off

      // sing national anthem for gold medal
      Serial2.write(217); // set 5th scale
      Serial2.write(224); // each line is a note
      Serial2.write(231);
      Serial2.write(220);
      Serial2.write(224);
      Serial2.write(227);
      Serial2.write(231);
      Serial2.write(224);
      Serial2.write(231);
      Serial2.write(224);
      Serial2.write(226);
      Serial2.write(231);
      Serial2.write(224);
      Serial2.write(227);
      Serial2.write(231);
      Serial2.write(224);
      Serial2.write(231);
      Serial2.write(224);
      Serial2.write(231);
      Serial2.write(220);
      Serial2.write(224);
      Serial2.write(227);
      Serial2.write(231);
      Serial2.write(224);
      Serial2.write(231);
      delay(50000);
    }
    else if(incoming=='1'){ // if silver, blinks once
      Serial2.print('1'); // print team score on LCD
      digitalWrite(4, HIGH); // front red LED turns on
      delay(500); // remains lit for 500 ms
      digitalWrite(4, LOW); // front red LED turns off

      // light show for silver medal
      for (int i=0;i<100;i++){ // loops 100 times
        digitalWrite(5, HIGH); // green LED blinks for 0.5 s
        delay(500);
        digitalWrite(5,LOW);
        digitalWrite(6,HIGH); // yellow LED blinks for 0.5 s
        delay(500);
        digitalWrite(6,LOW);
      }
      delay(500000);
    }
    else if(incoming=='0'){ // if bronze, doesn't blink
      Serial2.print('0'); // print team score on LCD
      delay(2000);
      // dance for bronze medal
      servoLeft.attach(13); // attach pin 12 to left motor
      servoRight.attach(12); // attach pin 13 to right motor
      // needs to back up first
      servoRight.write(1450);
      servoLeft.write(1550);
      delay(2000);
      // now dance
      servoRight.write(1550); // bot spins
      servoLeft.write(1550);
      delay(50000);
    }
delay(1000000);
}
