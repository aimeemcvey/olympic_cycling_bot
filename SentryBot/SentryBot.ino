#include <Wire.h>
#define Addr 0x1E //address of 3 axis compass module
#define MagRed 2 //sensing LED
#define MagYellow 3 //sensing LED
#define MagGreen 4 //sensing LED
#define Rx 15 //XBee Rx
#define Tx 14 //XBee Tx
#define Green 5 //communication LED
#define Red 6 //communication LED
#define buttonPin 7 //push button
#define rr 22 //right QTI
#define cr 24 //center QTI
#define ll 26 //left QTI
#include <Servo.h> // Include servo library


int C = 1108.73; //frequencies of musical notes
int G = 1661.22;
int A = 1760;
int E = 1318.51;
int D = 1244.51;



Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
int buttonState = 0;
int lb; //left boolean
int cb; //center boolean
int rb; //right boolean
int stage = 1; //indicates which task bot is to perform
int dir = 0; //which way bot is going around circle
int x, y, z; //direction of mag field
int score = 0; //final score
int value; //individual score
void setup() {
  Serial.begin(9600); //initialize
  Serial3.begin(9600);
  delay(100);
  pinMode(buttonPin, INPUT);
  pinMode(Green, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(rr, INPUT);
  pinMode(cr, INPUT);
  pinMode(ll, INPUT);
  pinMode(MagRed, OUTPUT);
  pinMode(MagYellow, OUTPUT);
  pinMode(MagGreen, OUTPUT);
  digitalWrite(MagRed, LOW);
  digitalWrite(MagYellow, LOW);
  digitalWrite(MagGreen, LOW);
  digitalWrite(Green, LOW);
  digitalWrite(Red, LOW);
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  // Power up delay
  Wire.begin();
  // Set operating mode to continuous
  Wire.beginTransmission(Addr);
  Wire.write(byte(0x02));
  Wire.write(byte(0x00));
  Wire.endTransmission();
  delay(500);
}
void loop() {
  if (stage == 1) { //line following and communication
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      digitalWrite(Green, 1);
      Serial3.print('g');
    }
    else {
      digitalWrite(Green, 0);
    }

    if (Serial3.available()) {
      digitalWrite(Red, 1);
      char incoming = Serial3.read();
      Serial.println(incoming);
    }
    else {
      digitalWrite(Red, 0);
    }
    long l = decayTime(ll);
    long c = decayTime(cr);
    long r = decayTime(rr); //determine values for QTI sensors
    Serial.print("l= ");
    Serial.print(l);
    Serial.print("c= ");
    Serial.print(c);
    Serial.print("r= ");
    Serial.print(r);

    if (l >= 500) //assign variables based on threshhold
    {
      lb = 1;
    }
    else
    {
      lb = 0;
    }
    if (c >= 500)
    {
      cb = 1;
    }
    else
    {
      cb = 0;
    }
    if (r >= 500)
    {
      rb = 1;
    }
    else
    {
      rb = 0;
    }
    Serial.print("\nl = ");
    Serial.print(l);
    Serial.print("\nc = ");
    Serial.print(c);
    Serial.print("\nr = ");
    Serial.print(r);
    if (cb == 1) {
      servoLeft.writeMicroseconds(1700); //center
      servoRight.writeMicroseconds(1300);
    }
    if (lb == 1) {
      servoLeft.writeMicroseconds(1400); //hi left
      servoRight.writeMicroseconds(1300);
      dir--;
      delay(50);
    }
    else if (rb == 1) {
      servoLeft.writeMicroseconds(1700); //hi right
      servoRight.writeMicroseconds(1600);
      dir++;
      delay(50);
    }
    if (lb + cb + rb > 1) //bot reached hash
    {
      servoLeft.writeMicroseconds(1500);
      servoRight.writeMicroseconds(1500); //pause
      delay(1000);
      if (dir < 0)
      {
        servoLeft.writeMicroseconds(1300); //hi left
        servoRight.writeMicroseconds(1300);
      }
      else if (dir > 0)
      {
        servoLeft.writeMicroseconds(1700); //hi right
        servoRight.writeMicroseconds(1700);
      }
      delay(670);
      servoLeft.writeMicroseconds(1500);
      servoRight.writeMicroseconds(1500);
      stage = 2; //finished line following
      delay(500);
    }

    delay(50); //delay should be 50
  }
  else if (stage == 2) { //sensing
    // Initiate communications with compass
    Wire.beginTransmission(Addr);
    Wire.write(byte(0x03));
    // Send request to X MSB register
    Wire.endTransmission();
    Wire.requestFrom(Addr, 6);
    // Request 6 bytes; 2 bytes per axis
    if (Wire.available() <= 6) {  // If 6 bytes available
      x = Wire.read() << 8 | Wire.read();
      z = Wire.read() << 8 | Wire.read();
      y = Wire.read() << 8 | Wire.read();
    }
    // Print raw values
    Serial.print("X=");
    Serial.print(x);
    Serial.print(", Y=");
    Serial.print(y);
    Serial.print(", Z=");
    Serial.println(z);
    delay(500);

    if (abs(y) < 300) { //white ball sensed
      digitalWrite(MagRed, HIGH);
      Serial3.print('f');
      Serial3.print('f');
      Serial3.print('f'); //send f to serial monitor
      value = 0; //individual score is 0
      digitalWrite(Green, HIGH);
      delay(1000);
      stage = 3; //sensing complete
    }
    else {
      digitalWrite(MagRed, LOW);
    }
    if (abs(y) > 300 && abs(y) < 900) { //grey ball sensed
      digitalWrite(MagYellow, HIGH);
      Serial3.print('e');
      Serial3.print('e');
      Serial3.print('e'); //send e to serial monitor
      value = 1; //individual score is 1
      digitalWrite(Green, HIGH);
      delay(1000);
      stage = 3; //sensing complete
    }
    else {
      digitalWrite(MagYellow, LOW);
    }
    if (abs(y) > 900) { //black ball sensed

      digitalWrite(MagGreen, HIGH);
      Serial3.print('d');
      Serial3.print('d');
      Serial3.print('d'); //send d to serial monitor
      value = 2; //individual score is 2
      digitalWrite(Green, HIGH);
      delay(1000);
      stage = 3; //sensing complete
    }
    else {
      digitalWrite(MagGreen, LOW);
    }

  }
  else if (stage == 3) { //determine final score and send to bots
    digitalWrite(Green, LOW);
    digitalWrite(MagGreen, LOW);
    digitalWrite(MagYellow, LOW);
    digitalWrite(MagRed, LOW); //reset all LEDs

    judgeCode(); //run judge code
    Serial3.print(score); //print total score
    score = score % 3; //mod 3 of score to determine final score

    Serial3.print(score); //send score to bots
    delay(500);
    Serial3.print(score);
    delay(500);
    Serial3.print(score);
    delay(500);
    Serial3.print(score);
    delay(500);
    Serial3.print(score);
    delay(500);

    Serial3.print("score");

    stage = 4; //scoring complete


  }
  else if (stage == 4) { //reset all LEDs
    digitalWrite(Green, LOW);
    digitalWrite(Red, LOW);
    digitalWrite(MagGreen, LOW);
    digitalWrite(MagRed, LOW);
    digitalWrite(MagYellow, LOW);

    stage = 5; //reset complete

  }
  else if (stage == 5) { //celebration time

    if (score == 0)
    {


      while (1) //dance
      {
        servoLeft.writeMicroseconds(1400); //hi left
        servoRight.writeMicroseconds(1300);
        delay(100);
        servoLeft.writeMicroseconds(1700); //hi right
        servoRight.writeMicroseconds(1600);
        delay(100);
      }
    }
    else if (score == 1)
    {


      while (1) //light show
      {
        digitalWrite(MagRed, HIGH);
        delay(25);
        digitalWrite(MagRed, LOW);
        digitalWrite(MagGreen, HIGH);
        delay(25);
        digitalWrite(MagGreen, LOW);
        digitalWrite(MagYellow, HIGH);
        delay(25);
        digitalWrite(MagYellow, LOW);
      }
    }
    else if (score == 2)
    {


      while (1) //national anthem
      {
        tone(9, C, 500);
        delay(650);
        tone(9, G, 500);
        delay(650);
        tone(9, A, 500);
        delay(650);
        tone(9, C, 500);
        delay(650);
        tone(9, E, 500);
        delay(650);
        tone(9, G, 500);
        delay(650);
        tone(9, C, 500);
        delay(650);
        tone(9, G, 500);
        delay(650);
        tone(9, C, 500);
        delay(650);
        tone(9, D, 500);
        delay(650);
        tone(9, G, 500);
        delay(650);
        tone(9, C, 500);
        delay(650);
        tone(9, E, 500);
        delay(650);
        tone(9, G, 500);
        delay(650);
        tone(9, C, 500);
        delay(650);
        tone(9, G, 500);
        delay(650);
        tone(9, C, 500);
        delay(650);
        tone(9, G, 500);
        delay(650);
        tone(9, A, 500);
        delay(650);
        tone(9, C, 500);
        delay(650);
        tone(9, E, 500);
        delay(650);
        tone(9, G, 500);
        delay(650);
        tone(9, C, 500);
        delay(650);
        tone(9, G, 500);
        delay(650);
      }
    }
  }
}
long decayTime(int pin) //decay time code to convert raw QTI readings
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(10);
  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);
  long time = micros();
  while (digitalRead(pin));
  time = micros() - time;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  return time;
}
void judgeCode() //determines total score
{

  char myVals[5] = "pqrst"; //array to hold scores from each bot

  int L = 0;
  int M = 0;
  int N = 0;
  int O = 0;
  int P = 0; //boolean values to determine when a score from a bot is received
  while ((L + M + N + O + P) < 5) //continue until all scores have been received
  {
    if (Serial3.available()) {
      char hello = Serial3.read(); //read incoming signal
      if (hello == 'a')
      {
        myVals[1] = 'a';
        L = 1; //score received
      }
      if (hello == 'b')
      {
        myVals[1] = 'b';
        L = 1;
      }
      if (hello == 'c')
      {
        myVals[1] = 'c';
        L = 1;
      }
      if (hello == 'g')
      {
        myVals[2] = 'g';
        M = 1;
      }
      if (hello == 'h')
      {
        myVals[2] = 'h';
        M = 1;
      }
      if (hello == 'i')
      {
        myVals[2] = 'i';
        M = 1;
      }
      if (hello == 'j')
      {
        myVals[3] = 'j';
        N = 1;
      }
      if (hello == 'k')
      {
        myVals[3] = 'k';
        N = 1;
      }
      if (hello == 'l')
      {
        myVals[3] = 'l';
        N = 1;
      }
      if (hello == 'm')
      {
        myVals[4] = 'm';
        O = 1;
      }
      if (hello == 'n')
      {
        myVals[4] = 'n';
        O = 1;
      }
      if (hello == 'o')
      {
        myVals[4] = 'o';
        O = 1;
      }
      if (value == 0) //our own bot score does not need to be received
      {
        myVals[5] = 'f';
        P = 1;
      }
      if (value == 1)
      {
        myVals[5] = 'e';
        P = 1;
      }
      if (value == 2)
      {
        myVals[5] = 'd';
        P = 1;
      }
    }

  }

  for (int u = 1; u <= 4; u++) //read array and summate
  {

    if (myVals[u] == 'a')
    {
      score = score + 2; //2 points for a
    }
    else if (myVals[u] == 'b')
    {
      score = score + 1; //1 point for b
    }
    else if (myVals[u] == 'c')
    {
      score = score + 0; //0 points for c
    }
    //    else if (myVals[] == 'd') //this should work but doesn't
    //    {
    //      score = score + 2;
    //    }
    //    else if (myVals[5] == 'e')
    //    {
    //      score = score + 1;
    //
    //    }
    //    else if (myVals[5] == 'f')
    //    {
    //      score = score + 0;
    //    }
    else if (myVals[u] == 'g')
    {
      score = score + 2;
    }
    else if (myVals[u] == 'h')
    {
      score = score + 1;
    }
    else if (myVals[u] == 'i')
    {
      score = score + 0;
    }
    else if (myVals[u] == 'j')
    {
      score = score + 2;
    }
    else if (myVals[u] == 'k')
    {
      score = score + 1;
    }
    else if (myVals[u] == 'l')
    {
      score = score + 0;
    }
    else if (myVals[u] == 'm')
    {
      score = score + 2;
    }
    else if (myVals[u] == 'n')
    {
      score = score + 1;
    }
    else if (myVals[u] == 'o')
    {
      score = score + 0;
    }
  }
  if (myVals[5] == 'd') //add score of own bot
  {
    score = score + 2;
  }
  else if (myVals[5] == 'e')
  {
    score = score + 1;
  }
  else if (myVals[5] == 'f')
  {
    score = score + 0;
  }
}

