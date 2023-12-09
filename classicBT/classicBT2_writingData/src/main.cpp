#include <Arduino.h>

#define SEC 10

// pir fields
const int sensor = 18;

// led fields
const int led   = 22;
const long blinkTime      = 1000;
int digitalState = LOW; // apparently hi/lo use 16 bits :/
// no negative time/quickly adds up 
unsigned long lastBlink   = 0;

// timer fields
unsigned long now = millis();
unsigned long lastTrigger =0;
boolean startTimer        = false;
boolean motion            = false;

// method declerations
void IRAM_ATTR detectsMovement();

void setup() {
  
  Serial.begin(115200);

  // PIR mode set
  pinMode(sensor, INPUT_PULLUP);

  // assign interrupt with sensor input
  attachInterrupt(digitalPinToInterrupt(sensor), 
  detectsMovement,  // invoke method 
  FALLING);         
  // note: more successful tracking using falling time
  // the output signal is a waveform so both should work equally well
  
  /*
  
  1) if set to H pir will detect less [?] because signal is LOW for
  duration of timer[?] and thus no new movement will be detected 
  until countdown is over? So L is better for repetetive movement?

  2) interrupt falling edge detection lasts how long? Tr time?

  */ 
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

}

void loop() {
  unsigned long currentMillis = millis();

  // // if it has been 1 second since last blink
  // if(currentMillis - lastBlink >= blinkTime){ // blink time will be 1 second
  //   lastBlink = currentMillis;

  //   if(digitalState == LOW){
  //     digitalState = HIGH;
  //   } else{
  //     digitalState = LOW;
  //   }

  //   digitalWrite(led, digitalState);
  now = millis();
  //Serial.println(now);

  if(digitalRead(led) == HIGH && (motion == false)){
    Serial.println("motion detected");
    motion = true;
  }

  // if motion has been detected 
  // and the time difference between trigger and now has
  // been 1 second, the print motion stopped
  if(startTimer && (now - lastTrigger > (1000))){ //10,000 is too long, makes it feel like dookie
    Serial.println("motion stopped");
    digitalWrite(led, LOW);
    startTimer = false;
    motion = false;
  }


  
}



// force into ram, not flash
// req'd for interrupts, If the method is not tagged with IRAM_ATTR 
//it will NOT be called during operations involving flash read/write
void IRAM_ATTR detectsMovement(){
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger= millis();
  //Serial.println(lastTrigger);
  
}
