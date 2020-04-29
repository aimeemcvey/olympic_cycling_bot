//#include <Servo.h>
//Servo motorLeft;
//Servo motorRight;
int t = 30

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(RCTime(13));
  delay(20);
}
  long RCTime(int pin){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(1);
    pinMode(pin, INPUT);
    digitalWrite(pin, LOW);
    long time = micros();
    while(digitalRead(pin));
    time = micros()-time;
    return time;
  }
