# include <stdio.h> 
# include <time.h> 


// these load specific methods from the ENGR101 library 
extern "C" int init_hardware();
extern "C" int Sleep ( int sec , int usec );
extern "C" int set_motor ( int motor , int dir , int speed );

int main(){ 
	// This sets up the RPi hardware and ensures 
	// everything is working correctly 
	init_hardware();

while (1) {
	set_motor (1 , 1, 255);

	set_motor (2 , 2, 255);
}

return 0;}
