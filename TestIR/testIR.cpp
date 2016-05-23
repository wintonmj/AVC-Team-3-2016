#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int read_analog(int ch_adc);
extern "C" int Sleep(int sec, int usec);

int main(){

    //This sets up the RPi hardware and ensures
    //everything is working correctly
    init(0);

    int adc_reading;
    int adc_reading2;
    // int adc_reading3;

    //Prints read analog value
    for (int i = 0; i < 100; i++) {

        //Reads from Analog Pin 0 (A0) through A7
        adc_reading = read_analog(0);
        adc_reading2 = read_analog(1);
        //adc_reading3 = read_analog(0); - Not added yet

        printf("Sensor 1: %d    Sensor 2: %d\n",adc_reading, adc_reading2);
        //Waits for 0.5 seconds (500000 microseconds)
        Sleep(0,500000);
    }

return 0;
}
