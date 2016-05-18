#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row, int col, int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor, int speed);
extern "C" int connect_to_server(char const server_addr[15], int port);
extern "C" int send_to_server(char const message[24]);
extern "C" int receive_from_server(char const message[24]);

#define NTP 16
    int DEBUG = 1;
    int width = 320;
    int height = 240;
    double dx = width / NTP;
    double dy = height / NTP;
    int midPic[NTP];
    int leftPic[NTP];
    int rightPic[NTP];
    int zeroCentre[] = {-8,-7,-6,-5,-4,-3,-2,-1,1,2,3,4,5,6,7,8};
    int n_whites_mid = 0;
    int n_whites_left = 0;
    int n_whites_right = 0;
    int method;
    int flag = 1;

    // PID

    // CONSTANTS
    double Pconstant = 7.5; // correcting speed (proportional to distance away from line)
    double Dconstant = 1080; // Change later

    // PROPORTIONAL CORRECTION
    double error_mid = 0.0;
    double Perror_mid = 0.0;
    double proportional = error_mid * Pconstant; // error correction for adjusting motor speed


    // DERIVATIVE CORRECTION
    struct timespec now;
    double now_ms = 0;
    double prev_ms = 0;
    double delta_ms = 0;
    double derivative = 0;

    double Tcorrection = proportional + derivative;

    //Motor Control
    int speed = 0;
    double v_left = speed + (Tcorrection); //speed of left motor
    double v_right = speed - (Tcorrection); // speed of right motor

int main() {

    init(0);
    method = 2;
    int string = 555555555555555;
/*
    //connects to server with the ip address 130.195.6.196
    connect_to_server("130.195.6.196", 1024);
    //sends a message to the connected server
    send_to_server("Please"); // "Please is the 'codeword', may change
    //receives message from the connected server
    char message[24];
    receive_from_server(message);
    send_to_server(message); // send password back to server to open up gate.

*/
    // Sets motors to speed = 100 for 2 seconds to boost through gate then lowers speed for PID control
    speed = 100; // boost through gate
    v_left = speed + Tcorrection; // speed of left motor
    v_right = speed - Tcorrection; // speed of right motor
    set_motor(1,v_left);
    set_motor(2,v_right);
    Sleep(2,000000);

    speed = 45;
    v_left = speed + Tcorrection; // speed of left motor
    v_right = speed - Tcorrection; // speed of right motor

    DEBUG = 1;
    method = 2;
    
    // Method for completing quadrant 1
    while (method == 1) {
        printf("%d",string);
        if (flag == 1) {
            delta_ms = 1;
            flag++;
        }

        clock_gettime(CLOCK_REALTIME, & now);
        prev_ms = (now.tv_sec * 1000 + (now.tv_nsec + 500000) / 1000000); //convert to milliseconds
        if (DEBUG == 2) {
            printf("\n PREV  %f", prev_ms);
        }

        take_picture();
        n_whites_mid = 0;
        for (int i = 0; i < 16; i++) {
            int x = dx * i + (dx / 2);
            midPic[i] = get_pixel(x, 120, 3);
            if (midPic[i] > 100) {
                // 1 = white
                midPic[i] = 1;
                n_whites_mid++;
            }
            else {
                // 0 = black
                midPic[i] = 0;
            }
            if (DEBUG == 1) {
                printf("%d ", midPic[i]);
            }
        }
        // reset variables
        error_mid = 0.0;
        for (int l = 0; l < 16; l++) {
            error_mid = error_mid + zeroCentre[l] * midPic[l];
        }
        if (n_whites_mid!= 0)
        {
            error_mid = error_mid / ((double) n_whites_mid);
            proportional = error_mid * Pconstant;
            derivative = ((error_mid - Perror_mid) / delta_ms) * Dconstant;
            if (DEBUG == 1) {
                printf("\n");
            }
            v_left = speed + Tcorrection;
            v_right = speed - Tcorrection;
        }

		// control statement to change to quadrant 2/3 methodology
        else if (n_whites_mid == 16 && error_mid == 0) {
            method = 2;
            speed = 20; // Set speed to drop to when it recognises the line.
			// CHANGE THE OTHER CONSTANTS TO REFLECT THE SPEED DROP FOR QUADRANT 2/3.
			// LETS GET IT WORKING AT A LOW SPEED THEN BUILD UP AND FIDDLE LATER
			Pconstant = 0;
			Dconstant = 0;
			// resetting these values for new method in quadrant 2/3
			now_ms = 0;
			prev_ms = 0;
			delta_ms = 0;
			derivative = 0;
        }

        else {
            if (DEBUG == 1) {
                printf("No line detected\n");
            }
            v_left = 1.5 * -speed; ///////
            v_right = 1.5 * -speed; ////////
        }
        if (DEBUG == 2) {
            printf("\n ERROR %f", error_mid);
            printf("\n PERROR %f", Perror_mid);
        }
        Perror_mid = error_mid;
        clock_gettime(CLOCK_REALTIME, & now);
        now_ms = (now.tv_sec * 1000 + (now.tv_nsec + 500000) / 1000000); //convert to milliseconds
        delta_ms = now_ms - prev_ms;
        if (DEBUG == 2) {
            printf("\n NOW  %f", now_ms);
            printf("\n DELT %f", delta_ms);
            printf("\n DERIV %f", derivative);
            printf("\n V_LEFT %f", v_left);
            printf("\n V_RIGHT %f", v_right);
        }
        set_motor(1, v_left);
        set_motor(2, v_right);
    }


















    // method for completing quadrant 2/3
    while (method == 2) {

	speed = 40;
	v_left = 0;
	v_right = 0;

    clock_gettime(CLOCK_REALTIME, & now);
    prev_ms = (now.tv_sec * 1000 + (now.tv_nsec + 500000) / 1000000); //convert to milliseconds
    if (DEBUG == 2) {
        printf("\n PREV  %f", prev_ms);
    }
    printf("%d",string);
    take_picture();
    n_whites_mid = 0;
    n_whites_left = 0;
    n_whites_right = 0;
        for (int i = 0; i < 16; i++) {
            int x = dx * i + (dx / 2);
	    int y = dy * i + (dy /2 );
            midPic[i] = get_pixel(x, 1, 3);
            leftPic[i] = get_pixel(10,y , 3); // looking at pixels 100 pixels higher than middle (looking ahead).
	    rightPic[i] = get_pixel(310,y,3);
            if (midPic[i] > 100) {
                // 1 = white
                midPic[i] = 1;
                n_whites_mid++;
            }
            else if (midPic[i] < 100) {
                // 0 = black
                midPic[i] = 0;
            }
            else if (leftPic[i] > 100) {
                // 1 = white
                leftPic[i] = 1;
                n_whites_left++;
            }
            else if (leftPic[i] < 100) {
                // 0 = black
                leftPic[i] = 0;
            }

	    else if (rightPic[i] > 100) {
		// 1 = white
		rightPic[i] = 1;
		n_whites_right++;
	    }

	    else if (rightPic[i] < 100) {
		rightPic[i] = 0; 
	   }

            // Debugging
            if (DEBUG == 1) {
                printf("%d ", midPic[i]);
                printf("              ");
                printf("%d ", leftPic[i]);
		printf("              ");
		printf("%d", rightPic[i]);
            }
        }
        error_mid = 0;

        // convert into error used for proportional control
        for (int l = 0; l < 16; l++) {
            error_mid = error_mid + zeroCentre[l] * midPic[l];
        }

        // Begin checking for each type of corner/intersection:
        if (n_whites_mid > 14 && error_mid == 0 && n_whites_left!=0 && n_whites_right!=0) {
		v_left = 0;
		v_right = speed;
	}
	set_motor(1,v_left);
	set_motor(1,v_right);
    }
}
