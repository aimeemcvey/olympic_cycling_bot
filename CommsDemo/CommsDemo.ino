#define Rx 15 // DOUT to pin 15
#define Tx 14 // DIN to pin 14
int buttonState = 0;

void setup() {
  Serial.begin(9600); // Set to No line ending;
  Serial3.begin(9600); // type a char, then hit enter
  delay(500);
  pinMode(2, INPUT); // set up push button
  pinMode(3, OUTPUT); // set up green LED
  pinMode(4, OUTPUT); // set up red LED
}
void loop() {
   if(digitalRead(2)==HIGH) { // executes if push button pressed
    char outgoing = 'A'; // transmits 'a'
    Serial3.print(outgoing); // prints 'a' on serial monitor
    digitalWrite(3, HIGH); // green LED turns on
    delay(250); // remains lit for 250 ms
    digitalWrite(3, LOW); // green LED turns off
   }
   if(Serial3.available()) { // executes if data available from sentry bot
    char incoming = Serial3.read(); // reads character, sends to XBee
    Serial.println(incoming); // prints incoming message on monitor
    digitalWrite(4, HIGH); // red LED turns on
    delay(250); // remains lit for 250 ms
    digitalWrite(4, LOW); // red LED turns off
   }
   delay(50);
   }
