// https://www.circuitbasics.com/how-to-read-user-input-from-the-arduino-serial-monitor/
//https://forum.arduino.cc/t/serial-input-basics-updated/382007
// putty cmd  sudo putty /dev/ttyUSB0 -serial -sercfg 115200,8,n,1,N

/*
   a simple BT classic program that allows seiral comms between phone and esp
   a motion sensor detects motion and lightens led,
   sending a ble notification to connected device
*/


#include <Arduino.h>
#include <BluetoothSerial.h>


// macro ensuring bt configs are defined (kinda redundant no?)
#if !defined(CONFIG_BT_ENABLED) || !defined (CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//instantiation
BluetoothSerial lilGuy;

//#define SEC 10


// pir fields
const int sensor = 18;

// led fields
const int led   = 22;
const int led2  = 4;
# define HFLAG "led_on"
# define LFLAG "led_off"
// just a lil macro to toggle led (testing with different port)
// although i thought duino environment couldnt digitalread digital output??
#define TOGGLE(x) digitalWrite(x, digitalRead(x) ? LOW : HIGH)
#define TOGGLE2(x) digitalWrite(x, digitalRead(x) ? HIGH: LOW)

const long blinkTime      = 1000;
int digitalState = LOW; // apparently hi/lo use 16 bits :/
// no negative time/quickly adds up 
unsigned long lastBlink   = 0;

// timer fields
unsigned long now = millis();
unsigned long lastTrigger =0;
boolean startTimer        = false;
boolean motion            = false;


// msg strings
String aWord = "";
char aLetter;
String tempString = "";

const byte wordLimit = 32;//array cells
char words[wordLimit];    //character array
boolean gotWord = false;  //input flag 




// method declerations
void IRAM_ATTR detectsMovement();
void getWord();
void printWords();

void setup() {
  
  Serial.begin(115200);
  delay(200);
  lilGuy.begin("lilDude");
  Serial.println("connect");

  // PIR mode set
  pinMode(sensor, INPUT_PULLUP);

  // assign interrupt with sensor input
  attachInterrupt(digitalPinToInterrupt(sensor), 
  detectsMovement,  // invoke method 
  FALLING);         
  /*
  note: more successful tracking using falling time (since pullup)
  the output signal is a waveform so both should work equally well
  
  1) if set to H pir will detect less [?] because signal is LOW for
  duration of timer[?] and thus no new movement will be detected 
  until countdown is over? So L is better for repetetive movement?

  2) interrupt falling edge detection lasts how long? Tr time?

  */ 

  // pir led
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  // toggle led
  pinMode(led2, OUTPUT);
  digitalWrite(led2, LOW);

}

void loop() {
  unsigned long currentMillis = millis();

  now = millis();

  getWord();
  printWords();



  if(digitalRead(led) == HIGH && (motion == false)){
    lilGuy.print("motion detected");
    motion = true;
  }

  // if motion has been detected 
  // and the time difference between trigger and now has
  // been 1 second, the print motion stopped
  if(startTimer && (now - lastTrigger > (1000))){ //10,000 is too long, makes it feel like dookie
    lilGuy.println("motion stopped");
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


void getWord(){
  memset(words, 0, sizeof(words)); // Clear the words array
  static byte index = 0;
  char delim = '\n';
  char letters;

  while(lilGuy.available() > 0  // while bt buffer has data
    && gotWord == false){       // and no word is obtained
      letters = lilGuy.read();  // read current character 
      if(letters != delim){     // if char is not the delim
        words[index] += letters;// add char to current index
        index ++;               // increase index
        if(index >= wordLimit){ // if end of array has been reached
          index = wordLimit -1; // keep resetting to last index until delim is obtained
          //Serial.println("word limit reached!\n");
          //lilGuy.println("Word limit reached!\n");
        }
      }
      else{
        gotWord = true;        // if delim obtained
        words[index] = '\0';    // null terminate the string
        // null terminator is required because it denotes
        // the end of a string but it is also req'd to 
        // reset the character array so that any characters
        // from previous strings aren't included
        index = 0;              // reset index otherwise s terminator does nothing
        //gotWord = true;         // set flag = 'i have a word'
        lilGuy.flush();
        Serial.flush();
      }
    }
}


void printWords(){
  String wordString = words;
  wordString.trim();

  // Serial acts up unless both cases have the bool flag
  // instead of just having it after if-statement
  if(gotWord == true){
    if(wordString == HFLAG || wordString == LFLAG){
      TOGGLE(led2);
      gotWord = false;
    }
    else{
      Serial.println(words);
      gotWord = false;
    }
  }
}     
// need to adjust flush method, currently, unless you give it 1 - 2 secs, serial 
// wont receive mssg                     