#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );
extern "C" int connect_to_server(char const  server_addr[15],int port);
extern "C" int send_to_server(char const message[24]);
extern "C" int receive_from_server(char const message[24]);

int DEBUG = 0;
#define NTP 16


// Global Variables

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
int method = 1;
int flag = 0;

// PID
double error_mid = 0.0;
double Perror_mid = 0.0;
double Pconstant = 7.5; // correcting speed (proportional to distance away from line)
double Dconstant = 1080; // Change later

struct timespec now;
double proportional = error_mid * Pconstant; // error correction for adjusting motor speed
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

    //connects to server with the ip address 130.195.6.196
    connect_to_server("130.195.6.196", 1024);
    //sends a message to the connected server
    send_to_server("Please"); // "Please is the 'codeword', may change
    //receives message from the connected server
    char message[24];
    receive_from_server(message);
    send_to_server(message); // send password back to server to open up gate.

    Sleep(1,000000);
    speed = 95;

    v_left = speed + Tcorrection; // speed of left motor
    v_right = speed - Tcorrection; // speed of right motor

    // Method for completing quadrant 1
    while (method == 1) {
	if (flag == 1) {
	    delta_ms = 1;
	    flag++;
	}
	clock_gettime(CLOCK_REALTIME, &now);
    	prev_ms = (now.tv_sec * 1000 + (now.tv_nsec + 500000) / 1000000); //convert to milliseconds
	if (DEBUG == 2){
	    //printf("\n PREV  %f", prev_ms);
    	}
	take_picture();
	n_whites_mid = 0;
	for (int i = 0; i < 16; i++) {
	    int x = dx * i + (dx/2);
	    midPic[i] = get_pixel(x,120,3);
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

        if (n_whites_mid!=0 && n_whites_mid != 16)
        {
            error_mid = error_mid/((double)n_whites_mid);
            proportional = error_mid * Pconstant;
	    derivative = ((error_mid-Perror_mid)/delta_ms) * Dconstant;
	    Tcorrection = proportional + derivative;

	    if (DEBUG == 1) {
		printf("\n");
	    }
	    v_left = speed + Tcorrection;
            v_right = speed - Tcorrection;
	    if (v_left > 255) {
		v_left = 255;
	    }

	    else if (v_right > 255) {
	    	v_right = 255;
	    }
	}

        else if (n_whites_mid == 16) {
            v_left = speed;
            v_right = speed;
            Sleep(1,000000);
            method = 2;
            speed = 0;
        }
        else if (n_whites_mid == 0){
		if (DEBUG == 1) {
                    printf("No line detected\n");
	}
		v_left = -1 * speed;
		v_right =-1 * speed;
        }

	if (DEBUG == 2) {
		//printf("\n ERROR %f", error_mid);
		//printf("\n PERROR %f", Perror_mid);
	}

	Perror_mid = error_mid;
	clock_gettime(CLOCK_REALTIME, &now);
        now_ms = (now.tv_sec * 1000 + (now.tv_nsec + 500000) / 1000000); //convert to milliseconds

	delta_ms = now_ms - prev_ms;
		if (DEBUG == 2){
			//printf("\n NOW  %f", now_ms);
			//printf("\n DELT %f", delta_ms);
			//printf("\n DERIV %f", derivative);
			printf("\n V_LEFT %f", v_left);
			printf("\n V_RIGHT %f\n", v_right);
			//printf("\n SPEED %d", speed);
		}
        set_motor(1,v_left);
        set_motor(2,v_right);
}




































    // method for completing quadrant 2/3
	while (method == 2) {

clock_gettime(CLOCK_REALTIME, &now);
    	prev_ms = (now.tv_sec * 1000 + (now.tv_nsec + 500000) / 1000000); //convert to milliseconds
	if (DEBUG == 2){
	    //printf("\n PREV  %f", prev_ms);
    	}
	take_picture();
	n_whites_mid = 0;
	n_whites_left = 0;
	n_whites_right = 0;

	for (int i = 0; i < 16; i++) {
	    int x = dx * i + (dx/2);
	    int y = dy * i + (dy/2);
	    midPic[i] = get_pixel(x,120,3);
	    leftPic[i] = get_pixel(10,y,3);
	    rightPic[i] = get_pixel(310,y,3);

	    if (midPic[i] > 100) {
                // 1 = white
	        midPic[i] = 1;
	        n_whites_mid++;
	    }
	    else {
            // 0 = black
            midPic[i] = 0;
	    }

        if (leftPic[i] > 100) {
            // 1 = white
	        leftPic[i] = 1;
	        n_whites_left++;
	    }
	    else {
            // 0 = black
            leftPic[i] = 0;
	    }

	       if (rightPic[i] > 100) {
           // 1 = white
	        rightPic[i] = 1;
	        n_whites_right++;
	    }
	    else {
            // 0 = black
            rightPic[i] = 0;
	    }
	}
        // reset variables
        error_mid = 0.0;
        for (int l = 0; l < 16; l++) {
            error_mid = error_mid + zeroCentre[l] * midPic[l];
        }


        if (n_whites_mid!=0 && n_whites_mid != 16)
        {
            error_mid = error_mid/((double)n_whites_mid);
            proportional = error_mid * Pconstant;
            derivative = ((error_mid-Perror_mid)/delta_ms) * Dconstant;
            Tcorrection = proportional + derivative;
            v_left = speed + Tcorrection;
            v_right = speed - Tcorrection;

            if (v_left > 255)
            {
                v_left = 255;
            }

            else if (v_right > 255)
            {
                v_right = 255;
            }

            if (n_whites_left > 3) {
                v_left = 0;
                v_right = speed;
                Sleep(1,000000);
                v_left = 0;
                v_right = 0;
            }

            if (n_whites_right > 3) {
                v_left = speed;
                v_right = 0;
                Sleep(1,000000);
                v_left = 0;
                v_right = 0;
            }
        }

        else if (n_whites_mid == 0){
            v_left = -1 * speed;
            v_right =-1 * speed;
        }

	Perror_mid = error_mid;
	clock_gettime(CLOCK_REALTIME, &now);
    now_ms = (now.tv_sec * 1000 + (now.tv_nsec + 500000) / 1000000); //convert to milliseconds
	delta_ms = now_ms - prev_ms;
    set_motor(1,v_left);
    set_motor(2,v_right);
    }
}
