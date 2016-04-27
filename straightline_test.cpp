# include <stdio.h> 
# include <time.h> 

\\ these load specific methods from the ENGR101 library 
extern "C" int InitHardware (); 
extern "C" int Sleep ( int sec , int usec ); 
extern "C" int SetMotor ( int motor , int dir , int speed ); 

extern "C" int InitHardware (); 
extern "C" int ReadAnalog ( int ch_adc ); 
extern "C" int Sleep ( int sec , int usec ); 

int main (){ 
	// This sets up the RPi hardware and ensures 
	// everything is working correctly 
	InitHardware (); 

While (1) {
	SetMotor (1 , 1, 255);      
	
	SetMotor (2 , 2, 255);         
}

return 0;}
