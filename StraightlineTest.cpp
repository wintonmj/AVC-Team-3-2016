# include <stdio.h>
# include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int Sleep( int sec , int usec );
extern "C" int set_motor(int motor , int speed );

int main (){

   // This sets up the RPi hardware and ensures
   // everything is working correctly
   // Value of 1 means debugging information is displayed
   init(1);

   // Sets the motor connected to pin 1 to rotate
   // in one direction at MAX speed .
   set_motor(1, 255); // pin 1 = right hand motor (red and purple wires)

   // Sets the motor connected to pin 1 to rotate
   // in the opposite direction at 50 % speed .
   set_motor(2, 255); // pin 2 = left hand motor (green and yellow wires)
   Sleep(1 ,000000);

   // Stops the motors from running
   set_motor(1,0);
   set_motor(2,0);
   
return 0;}
