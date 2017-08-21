#include "ARDUINO.h"
#include <Wire.h>

#include "oled.h"
#include "num.h"

#define LED_BUILTIN         16

#define NUM_WIDTH           32
#define NUM_HEIGHT          48

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

int count_display_num = 0;

void ssd1306_command(uint8_t c) {
  Wire.beginTransmission(SSD1306_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.write(c);
  Wire.endTransmission();
}

void DisplayNum(int dis_num){
  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(32);   // Column start address (0 = reset)
  ssd1306_command(64-1); // Column end address (127 = reset)

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(1); // Page start address (0 = reset)
  ssd1306_command(7); // Page end address

  for (uint16_t i=0; i<12; i++) {
    Wire.beginTransmission(SSD1306_I2C_ADDRESS);
    Wire.write(0x40);
    for (uint8_t x=0; x<16; x++) {
      Wire.write(num[x+(i*16)+((dis_num / 10) *(NUM_WIDTH * NUM_HEIGHT / 8))]);
    }
    Wire.endTransmission();
  }

  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(64);   // Column start address (0 = reset)
  ssd1306_command(96-1); // Column end address (127 = reset)

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(1); // Page start address (0 = reset)
  ssd1306_command(7); // Page end address

  for (uint16_t i=0; i<12; i++) {
    Wire.beginTransmission(SSD1306_I2C_ADDRESS);
    Wire.write(0x40);
    for (uint8_t x=0; x<16; x++) {
      // Wire.write(num[x+(i*16)+192]);
      Wire.write(num[x+(i*16)+((dis_num % 10) *(NUM_WIDTH * NUM_HEIGHT / 8))]);
    }
    Wire.endTransmission();
  }
}

void setup() {
  Wire.begin();

  // Init sequence
  ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
  ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
  ssd1306_command(0x80);                                  // the suggested ratio 0x80
  ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
  ssd1306_command(SSD1306_LCDHEIGHT - 1);
  ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  ssd1306_command(0x0);                                   // no offset
  ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
  ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
  ssd1306_command(0x14);
  ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
  ssd1306_command(0x00);                                  // 0x0 act like ks0108
  ssd1306_command(SSD1306_SEGREMAP | 0x1);
  ssd1306_command(SSD1306_COMSCANDEC);
  ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
  ssd1306_command(0x12);
  ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  ssd1306_command(0xCF);
  ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
  ssd1306_command(0xF1);
  ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
  ssd1306_command(0x40);
  ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
  ssd1306_command(SSD1306_DEACTIVATE_SCROLL);
  ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel

  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(0);   // Column start address (0 = reset)
  ssd1306_command(128-1); // Column end address (127 = reset)

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(0); // Page start address (0 = reset)
  ssd1306_command(7); // Page end address

  // I2C
   for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++) {
    // send a bunch of data in one xmission
    Wire.beginTransmission(0x3C);
    Wire.write(0x40);
    for (uint8_t x=0; x<16; x++) {
      Wire.write(0x00);
       i++;
    }
     i--;
    Wire.endTransmission();
   }

   DisplayNum(0);

   pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW){
      ledState = HIGH;
    }else{
      ledState = LOW;
    }

    count_display_num++;
    if(count_display_num > 99){
      count_display_num = 0;
    }

    DisplayNum(count_display_num);

  }
}
