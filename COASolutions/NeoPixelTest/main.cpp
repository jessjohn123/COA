/*
 * NeoPixelTest.cpp
 *
 * Created: 6/3/2016 5:28:46 PM
 * Author : NexusFS
 */ 

//#include <avr/io.h>
#include <Arduino.h>
#include <NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void colorWipe(uint32_t c, uint8_t wait);
void rainbow(uint8_t wait);
void rainbowCycle(uint8_t wait);
void theaterChase(uint32_t c, uint8_t wait);
void theaterChaseRainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos);

void setNeoPixel(uint8_t target, float heading, float distance)
{
    //Clearing LEDs
    for (int i = 0; i < 40; i++)
    {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        strip.show();
    }
    
    // add code here0
    int NUMLEDs = 6;
    
    if(distance < 100)
    {
        NUMLEDs = distance / 16.66;
    }
    
    uint32_t color = 0;
    if(target == 0)
    color = strip.Color(255,0,0);
    
    else if(target == 1)
    color = strip.Color(0,255,0);
    
    else if(target == 2)
    color = strip.Color(0,0,255);
    
    else if(target == 3)
    color = strip.Color(255,0,255);
    
    //More LEDs in the middle as you get farther less when you get closer
    for (int i = 18; i < 18 + NUMLEDs; i++)
    {
        strip.setPixelColor(i, color);
        strip.show();
    }
    
    
    //Which Direction Simple Arrows
    if((heading > 330) || (heading > 0 && heading < 30))
    {
        strip.setPixelColor(9, strip.Color(255, 255, 255));
        strip.setPixelColor(16, strip.Color(255, 255, 255));
        strip.setPixelColor(25, strip.Color(255, 255, 255));
    }
    
    else if(heading > 30 &&  heading < 60)
    {
        strip.setPixelColor(8, strip.Color(255, 255, 255));
        strip.setPixelColor(0, strip.Color(255, 255, 255));
        strip.setPixelColor(1, strip.Color(255, 255, 255));
    }
    
    else if(heading > 60 && heading < 120)
    {
        strip.setPixelColor(4, strip.Color(255, 255, 255));
        strip.setPixelColor(11, strip.Color(255, 255, 255));
        strip.setPixelColor(13, strip.Color(255, 255, 255));
    }
    
    else if(heading > 120 && heading < 150)
    {
        strip.setPixelColor(6, strip.Color(255, 255, 255));
        strip.setPixelColor(7, strip.Color(255, 255, 255));
        strip.setPixelColor(15, strip.Color(255, 255, 255));
    }
    
    else if(heading > 150 && heading < 210)
    {
        strip.setPixelColor(14, strip.Color(255, 255, 255));
        strip.setPixelColor(23, strip.Color(255, 255, 255));
        strip.setPixelColor(30, strip.Color(255, 255, 255));
    }
    
    else if(heading > 210 && heading < 240)
    {
        strip.setPixelColor(31, strip.Color(255, 255, 255));
        strip.setPixelColor(38, strip.Color(255, 255, 255));
        strip.setPixelColor(39, strip.Color(255, 255, 255));
    }
    
     else if(heading > 240 && heading < 300)
     {
         strip.setPixelColor(36, strip.Color(255, 255, 255));
         strip.setPixelColor(29, strip.Color(255, 255, 255));
         strip.setPixelColor(27, strip.Color(255, 255, 255));
     }
    
    else if(heading > 300 && heading < 330)
    {
        strip.setPixelColor(24, strip.Color(255, 255, 255));
        strip.setPixelColor(32, strip.Color(255, 255, 255));
        strip.setPixelColor(33, strip.Color(255, 255, 255));
    }
    
    strip.show();
}

int main(void)
{
    init();
    //uint32_t val = 123456;
    
    
    Serial.begin(115200);
    strip.begin();
    strip.setBrightness(20);
    strip.show(); // Initialize all pixels to 'off'
    unsigned int TimeOut = millis() + 100;
    int head = 0;
    /* Replace with your application code */
    while (true) 
    {
         // Some example procedures showing how to display to the pixels:
         //colorWipe(strip.Color(255, 0, 0), 50); // Red
         //colorWipe(strip.Color(0, 255, 0), 50); // Green
         //colorWipe(strip.Color(0, 0, 255), 50); // Blue
         //// Send a theater pixel chase in...
         //theaterChase(strip.Color(127, 127, 127), 50); // White
         //theaterChase(strip.Color(127,   0,   0), 50); // Red
         //theaterChase(strip.Color(  0,   0, 127), 50); // Blue
         //
         //rainbow(20);
         //rainbowCycle(20);
         //theaterChaseRainbow(50);
         if(TimeOut < millis())
         {
             TimeOut += 10;
             head++;
             if(head > 360)
                head = 0;
                
             Serial.println(head);
         }
         
         setNeoPixel(1, head, 500);
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256; j++) {
        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i+j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
    for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        for (int q=0; q < 3; q++) {
            for (unsigned int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, c);    //turn every third pixel on
            }
            strip.show();
            
            delay(wait);
            
            for (unsigned int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
    for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++) {
            for (unsigned int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
            }
            strip.show();
            
            delay(wait);
            
            for (unsigned int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
        } else if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
        } else {
        WheelPos -= 170;
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}