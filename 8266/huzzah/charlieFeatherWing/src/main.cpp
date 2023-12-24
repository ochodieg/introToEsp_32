#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>

// If you're using the full breakout...
//Adafruit_IS31FL3731 matrix = Adafruit_IS31FL3731();
// If you're using the FeatherWing version
Adafruit_IS31FL3731_Wing matrix = Adafruit_IS31FL3731_Wing();
// sets text size, origin, etc. to fit featherwing

// Adafruit_IS31FL3731 subclass only instantiates select functions

bool check;
int ran;
int msgCounter = 0;
int speedCounter = 60;

const char prt1[] = "<<<<<<<<<<<<--------------";
const char prt2[] = "-------------->>>>>>>>>>>>";
const char prt3[] = "12345678901234567890123456";
const char prt4[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char prt5[] = "{}{}{}{}{}{}{}{}{}{}{}{}{}";
const char prt6[] = "||||||||||||||||||||||||||";

String message[] = {prt1, prt2, prt3, prt4, prt5, prt6};


uint16_t msgWidth;

int randColor1;
int randColor2;
int randColor3;




static const uint8_t PROGMEM
  smile_bmp[] =
  { 0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100 },
  neutral_bmp[] =
  { 0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10111101,
    0b10000001,
    0b01000010,
    0b00111100 },
  frown_bmp[] =
  { 0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10011001,
    0b10100101,
    0b01000010,
    0b00111100 };


  uint8_t sweep[] = {1, 2, 3, 4, 6, 8, 10, 15, 20, 30, 40, 60, 60, 40, 30, 20, 15, 10, 8, 6, 4, 3, 2, 1};

void setup() {

  Serial.begin(9600);
  Serial.println("ISSI manual animation test");
  if (! matrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 Found!");
 
}


void loop() {


  // matrix.clear();
  // matrix.drawBitmap(0, 0, smile_bmp, matrix.width()-1,  matrix.height()-1, 255);
  // delay(500);
  
  // matrix.clear();
  // matrix.drawBitmap(3, 0, neutral_bmp, matrix.width()-1,  matrix.height()-1, 64);
  // delay(500);

  // matrix.clear();
  // matrix.drawBitmap(3, 0, frown_bmp, 8, 7, 32);
  // delay(500);

  // matrix.clear();
  // matrix.drawPixel(0, 0, 255);  
  // delay(500);

  matrix.clear();
  matrix.drawLine(0,0, matrix.width()-1, matrix.height()-1, 127);
  delay(3000);

  matrix.clear();
  matrix.drawFastHLine(0,
                      3,
                      16,
                      100);
  delay(5000);


  matrix.clear();
  matrix.drawRect(0,0, matrix.width(), matrix.height(), 100);
  delay(3000);
  matrix.fillRect(0,0, matrix.width()-4, matrix.height()-4, 255);
  delay(3000);





  // matrix.clear();
  // matrix.drawCircle(8,4, 4, 64);
  // matrix.drawCircle(8,4, 2, 32);
  // delay(5000);


  matrix.begin();         // initialize
  matrix.setRotation(0);  // redundant?
  matrix.setCursor(0,0);  // origin is in top left corner

  matrix.setTextSize(1);


  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely

  matrix.setTextColor(100);

  for (int8_t x=0; x>=-32; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("Hello");
    delay(100);
  }

  matrix.clear();
  matrix.setCursor(0,0);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely


  matrix.setTextColor(50); // this method uses RGB565
  // but because this this featherwing monochrome
  // it only takes 1 arg for 5 bits of red, so
  // its library treats this a "brightness" setting instead
  // no clue how code keeps track of what color the featherwing is tho
  // unless it just treats all three colors as 5-bit rather than 565?


  // matrix.setRotation(1); // rotates text by 90 degrees
  // this also rotates location of origin?
  
  for (int8_t x=30; x>=-96; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("THIS IS A TEST");
    delay(50);
  }

  matrix.clear();
  matrix.setCursor(0,0);
  
  matrix.drawPixel(0,0,255);

  

  delay(2500);

int set = 5;
for (uint8_t x=0; x<16; x++) {
  
  for (uint8_t y=0; y<9; y++) {
     if(x<=13){
      matrix.drawPixel(x, y, set);
      //delay(200);
    }else{
      matrix.drawPixel(x, y, 255);
    }
    
  }
  delay(20);
  set+=10;
}

delay(5000);


for (uint8_t x=0; x<16; x++) {
  for (uint8_t y=0; y<9; y++) {
    matrix.drawPixel(x, y, 5);
    
  }
  delay(10);
}
delay(1000);

while(1){
        int bright = 0;
        int amt = 0;
        // animate over all the pixels, and set the brightness from the sweep table
  for (uint8_t incr = 0; incr < 48; incr++)
    for (uint8_t x = 0; x < 16; x++){
      delay(5);
      amt++;
      bright = sweep[(x+amt+incr)%24];
      for (uint8_t y = 0; y < 9; y++)
        //matrix.drawPixel(x, y, sweep[(x+y+incr)%24]);
        matrix.drawPixel(x, y, bright);
  }
//matrix.clear();
//delay(20);
}
}


// for(uint8_t x =0; x<16; x++){

// }
  
  //   // animate over all the pixels, and set the brightness from the sweep table
  // for (uint8_t incr = 0; incr < 24; incr++)
  //   for (uint8_t x = 0; x < 16; x++)
  //     for (uint8_t y = 0; y < 9; y++)
  //       // matrix.drawPixel(x, y, sweep[(x+y+incr)%24]);
  //       matrix.fillRect(0,0, matrix.width()-5, matrix.height()-4, sweep[(x+y+incr)%24]);

  
  
///////////////////////////////////////////////////////////////
 
///////////////////////////////////////////////////////////////

//matrix.clear();
//matrix.setCursor(3,7);
/*
trying to make matrix expand from center 
but accidentally created a pulsing animation
that slows down 
*/
//4x8
//3x7
// int width = 4;
// int height = 3;
// int x = 6;  // y origin
// int y = 2;  // x origin
// int pwM = 0;
// int val = 0;
// while(1){
//   for(int i = 0; i < 24; i++){
//     // matrix.fillRect(0,0, matrix.width()+j, matrix.height()+k, sweep[(j+k+i)%24]);
//     //matrix.fillRect(x,y,width,height,sweep[(width+height+i)%24]);
//         pwM = map(pwM, val, 24, 0, 255);
//         matrix.fillRect(x,
//                     y, 
//                     width, 
//                     height, 
//                     pwM);
//     delay(2000);
//     val++;                                                                                                       
//     width += width;
//     height += height;
//     x--;  // origin should expand to compensate for
//     y--;  // fillrect function
//     // might be easier to just make a linear function
    
//     if (width<=-15){
//       x = 6;
//       y = 2;
//       width = 4;
//       height = 3;
//       matrix.clear();
//       pwM = 0;
//       val = 0;
//     }
//   }
// }


