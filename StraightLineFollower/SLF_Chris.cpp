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

int main() {

    init(0);

    int DEBUG = 0;
    int width = 320;
    //int height = 240;
    double dx = width / NTP;
    int midPic[NTP];
    int topPic[NTP];
    int zeroCentre[] = {-8,-7,-6,-5,-4,-3,-2,-1,1,2,3,4,5,6,7,8};
    int n_whites_mid = 0;
    int n_whites_top = 0;
    int method = 1;
    int flag = 1;

    // PID

    // CONSTANTS
    double Pconstant = 9.2; // correcting speed (proportional to distance away from line)
    double Dconstant = 1050; // Change later

    // PROPORTIONAL CORRECTION
    double error_mid = 0.0;
    double Perror_mid = 0.0;
    double proportional = error_mid * Pconstant; // error correction for adjusting motor speed

    // ERROR CALCULATIONS
    double error_top = 0.0;

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
    
    //connects to server with the ip address 130.195.6.196
    connect_to_server("130.195.6.196", 1024);
    //sends a message to the connected server
    send_to_server("Please"); // "Please is the 'codeword', may change
    //receives message from the connected server
    char message[24];
    receive_from_server(message);
    send_to_server(message); // send password back to server to open up gate.

    // Sets motors to speed = 100 for 2 seconds to boost through gate then lowers speed for PID control
    speed = 100; // boost through gate
    v_left = speed + Tcorrection; // speed of left motor
    v_right = speed - Tcorrection; // speed of right motor
    set_motor(1,v_left);
    set_motor(2,v_right);
    Sleep(2,000000);

    speed = 65;
    v_left = speed + Tcorrection; // speed of left motor
    v_right = speed - Tcorrection; // speed of right motor

    // Method for completing quadrant 1
    while (method == 1) {
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
      
    clock_gettime(CLOCK_REALTIME, & now);
    prev_ms = (now.tv_sec * 1000 + (now.tv_nsec + 500000) / 1000000); //convert to milliseconds
    if (DEBUG == 2) {
        printf("\n PREV  %f", prev_ms);
    }

    take_picture();
    n_whites_mid = 0;
    n_whites_top = 0;
        for (int i = 0; i < 16; i++) {
            int x = dx * i + (dx / 2);
            midPic[i] = get_pixel(x, 120, 3);
            topPic[i] = get_pixel(x, 20, 3); // looking at pixels 100 pixels higher than middle (looking ahead).
            if (midPic[i] > 100) {
                // 1 = white
                midPic[i] = 1;
                n_whites_mid++;
            }
            else if (midPic[i] < 100) {
                // 0 = black
                midPic[i] = 0;
            }
            else if (topPic[i] > 100) {
                // 1 = white
                topPic[i] = 1;
                n_whites_top++;
            }
            else if (topPic[i] < 100) {
                // 0 = black
                topPic[i] = 0;
            }

            // Debugging
            if (DEBUG == 1) {
                printf("%d ", midPic[i]);
                printf("              ");
                printf("%d ", topPic[i]);
            }
        }
        error_mid = 0;
        error_top = 0;

        // convert into error used for proportional control
        for (int l = 0; l < 16; l++) {
            error_mid = error_mid + zeroCentre[l] * midPic[l];
            error_top = error_top + zeroCentre[l] * topPic[l];
        }

        // Begin checking for each type of corner/intersection:
        if (error_top == 0 && n_whites_top != 0) {

            // If it's a:
            // 1. left turn with continuation.
            // 2. right turn with continuation.
            // 3. '+' junction with continuation.
            if (error_mid < 0 || error_mid == 0 || error_mid > 0) {
				// Go straight ahead (our current method for following a line should work???)
                v_left = speed + Tcorrection;
				v_right = speed - Tcorrection;
            }
        }

        else if (n_whites_top == 0) {

            // dead end
            if (n_whites_mid < 5) {
                // move forward off the line then when it loops it will notice it has lost the line
                // will therefore run code from further down. I.E turn left until found line.
                v_left = speed;
                v_right = speed;
            }

            // left hand turn
			if (error_mid < 0) {
                    // turn left until line found
                    v_left = -speed;
                    v_right = speed;
			}

			else if (error_mid > 0) {
			// turn right until line found

			}
		}

		// METHOD FOR COMPLETELY LOSING LINE
		if (n_whites_mid == 0 && n_whites_top == 0) {
			// spin around to the left until finds a white line then breaks
			v_left = -speed;
			v_right = speed;
		}

		// if the top array can find a line, then go forwards until middle array can find line
		else if (n_whites_top!=0 && n_whites_mid == 0) {
			// move the robot forward until mid array detects whiteness
			v_left = speed;
			v_right = speed;
		}

		else if (n_whites_mid!=0) {
			// reloops back up as it has refound the line and thus corrects itself using line correcting method
			if (DEBUG == 1) {
				printf("You have found the line again, recorrecting...\n");
			}
			v_left = speed + Tcorrection;
			v_right = speed + Tcorrection;
		}
	}
}