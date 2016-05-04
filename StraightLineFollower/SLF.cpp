#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );
extern "C" int connect_to_server(char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

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
    int method = 1;

    //Constants
    double Pconstant = 6; // correcting speed (proportional to distance away from line)

    double proportional = error * Pconstant; // error correction for adjusting motor speed

    int speed = 0;
    double v_left = speed + proportional; //speed of left motor
    double v_right = speed - proportional; // speed of right motor

    //connects to server with the ip address 130.195.6.196
    connect_to_server("130.195.6.196", 1024);
    //sends a message to the connected server
    send_to_server("Please"); // "Please is the 'codeword', may change
    //receives message from the connected server
    char message[24];
    receive_from_server(message);
    send_to_server(message); // send password back to server to open up gate.

    speed = 100; // boost through gate
    Sleep(3,000000);

    speed = 40;

    v_left = speed + proportional; // speed of left motor
    v_right = speed - proportional; // speed of right motor

    // Method for completing quadrant 1
    while (method == 1) {
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
            v_left = speed + proportional;
            v_right = speed - proportional;
        }

        else if (n_whites == 16) {
            method = 2;
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

    // method for completing quadrant 2/3
	while (method == 2) {

	}
}
