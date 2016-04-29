#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );

int DEBUG = 1;

int main() {
	init(0);
	int width = 320;
	//int height = 240;
	int ntp = 16;
	double dx = width / ntp;
	int whiteness[ntp];
	int zeroCentre[] = {-8,-7,-6,-5,-4,-3,-2,-1,1,2,3,4,5,6,7,8};
    int n_whites = 0;
    double error = 0.0; // Define later

/*	int lineStart;
	int lineEnd;
    double lineCentre;
    double arrayCentre = (ntp - 1)/2;
    double v_left = 128 + (k*(lineCentre-arrayCentre));
	double v_right = 128 - (k*(lineCentre-arrayCentre));
*/

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
        printf("%d ", whiteness[i]);
		}
		printf("");

        error = 0.0;
        for (int l = 0; l < 16; l++) {
            error = error + zeroCentre[l] * whiteness[l];
        }
        if (n_whites!=0)
        {
            error = error/((double)n_whites);
            Sleep(0,100000);
            printf("\n");
        }
        else {
            printf("No line detected\n");
        }
	}
}
