// c++ libraries
#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );

int main() {
	init(0); // Set to 1 for debugging, 0 otherwise.
	
	// Variables
	int width = 320;
	//int height = 240;
	int ntp = 16;
	double dx = width / ntp;
	int threshold = 100;
	int whiteness[ntp];
	int zeroCentre[] = {-8,-7,-6,-5,-4,-3,-2,-1,1,2,3,4,5,6,7,8};
    int n_whites = 0;
    double error = 0.0;
    	
    // Values for changing how quickly/slowly robot responds to line = PID.
    int correctingSpeed = 3;
    int speed = 40;
    	
    double v_left = speed + (error*(correctingSpeed)); // speed of left motor
	double v_right = speed - (error*(correctingSpeed)); // speed of right motor

	while (1) {
		take_picture();
		n_whites = 0;
		// Get the 'whiteness' of 16 pixels going across the image at equal intervals
		for (int i = 0; i < ntp; i++) {
			int x = dx * i + (dx/2);
			whiteness[i] = get_pixel(x,120,3);
			
	    	if (whiteness[i] > threshold) {
                // 1 = white
	           	whiteness[i] = 1;
	          	n_whites++;
	        }
	                
	        else {
               	// 0 = black
               	whiteness[i] = 0;
	        }
        printf("%d ", whiteness[i]);
		}
	printf("");
    
    error = 0.0;
    for (int l = 0; l < ntp; l++) {
        error = error + zeroCentre[l] * whiteness[l];
    }
    
    // deciding what the motors should do based upon the line
    if (n_whites!=0) {
    	error = error/((double)n_whites);
        Sleep(0,100000);
        printf("\n");
        v_left = speed + (error*(correctingSpeed));
        v_right = speed - (error*(correctingSpeed));
    }
    
    else {
      	// If no line detected, move backwards
       	printf("No line detected\n");
       	v_left = -speed;
       	v_right = -speed;
    }
    // making motors to turn/move
    set_motor(1,v_left);
    set_motor(2,v_right);
	}
}
