#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );

int DEBUG = 0;
#define NTP 16

int main() {
    init(0);
    int width = 320;
    //int height = 240;
    double dx = width / NTP;
    int whiteness[NTP];
    int zeroCentre[] = {-8,-7,-6,-5,-4,-3,-2,-1,1,2,3,4,5,6,7,8};
    int n_whites = 0;


    // Values for changing how quickly/slowly robot responds to line (PID)

    //double Perror = 0.0; // Collects error from previous cycle of loop to find derivative value for control. (PID)
    double error = 0.0; // current error in the loop. Error is the distance away from line (from -8 to 8)
 
    //Constants
    double Pconstant = 6.2; // correcting speed (proportional to distance away from line)
    /*
    double Iconstant = 0.0; // constant to multiply accumulated error by
    
    double integral = 0.0;
    */
    
    double proportional = error * Pconstant;
    
    /*
    integral = integral + error;
    integral = integral * Iconstant;
 
    // method for derivative is: 'error - previous error/time taken to get rate of change of error'
    // previous error would be getting the previous index of zeroCentre array once multiplied
    // time taken to get rate of change of error would be starting a timer when we get current error and assign it to variable.
    // and stop when previous error is assigned. May not be neccessary as the time to get rate of change of error might be very small.
    double derivative = error - Perror;
 
    // once the above systems are in place, the total correction = 'proportional + integral + derivative'
    // this total correction is just added or subtracted to speed of left/right wheel.
    double Tcorrection = proportional + integral + derivative;
 
    */
    
    int speed = 40;
 
    double v_left = speed + proportional; // speed of left motor
    double v_right = speed - proportional; // speed of right motor
    

	while (1) {
		take_picture();
		n_whites = 0;
		for (int i = 0; i < 16; i++) {
			int x = dx * i + (dx/2);
			whiteness[i] = get_pixel(x,120,3);
	                if (whiteness[i] > 100) {
                        // 1 = white
	                    whiteness[i] = 1;
	                    n_whites++;
	                }
	                else {
                        // 0 = black
                        whiteness[i] = 0;
	                }
	if (DEBUG ==1) {
		printf("%d ", whiteness[i]);
	}
		}
	//Perror = error;
	// reset variables
        error = 0.0;
        for (int l = 0; l < 16; l++) {
            error = error + zeroCentre[l] * whiteness[l];
        }
        if (n_whites!=0)
        {
            error = error/((double)n_whites);
            proportional = error * Pconstant;
            
	    if (DEBUG == 1) {
		printf("\n");
	    }

            v_left = speed + proportional;   ///////
            v_right = speed - proportional; ////////
        }
        else {
		if (DEBUG == 1) {
                printf("No line detected\n");
		}
                v_left = 1.5* -speed;   ///////
                v_right = 1.5* -speed; ////////
        }
           set_motor(1,v_left);
           set_motor(2,v_right);

	}
}
