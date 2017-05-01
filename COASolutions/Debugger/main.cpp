/*
 * Debugger.cpp
 *
 * Created: 6/3/2016 5:28:46 PM
 * Author : NexusFS
 */ 

//#include <avr/io.h>
#include <Arduino.h>

#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_SHIELD
#include <OneSheeld.h>


int main(void)
{
    init();
    OneSheeld.begin();
    //uint8_t val = 123456;
    //Logger.stop();
    //Logger.start();
    /* Replace with your application code */
    while (true) 
    {
       
        Terminal.println("12");
        delay(1000);
        
    }
}

