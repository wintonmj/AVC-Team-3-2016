#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" char set_pixel(int row, int col, char red, char green, char blue);
extern "C" int open_screen_stream();
extern "C" int update_screen();
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );

int main() {
	init(0);
	int width = 320;
	//int height = 240;
	int ntp = 16;
	double dx = width / ntp;
	int whiteness[ntp];
	int lineStart;
	int lineEnd;
    double lineCentre;
    double arrayCentre = (ntp - 1)/2;
    int k; // Define later
    double v_left = 128 + (k*(lineCentre-arrayCentre));
	double v_right = 128 - (k*(lineCentre-arrayCentre));

	open_screen_stream();

	while (1) {

		take_picture();
		for (int i = 0; i < 16; i++) {
			int x = dx * i + (dx/2);
			whiteness[i] = get_pixel(x,120,3);

	                if (whiteness[i] > 100) {
                        // 1 = white
	                    whiteness[i] = 1;
	                }
	                else {
                        // 0 = black
                        whiteness[i] = 0;
	                }

	                set_pixel(x,120 + 2,255,0,0);
          	        set_pixel(x,120 - 2,255,0,0);

		}

        for (int k = 0; k < 15; k++) {
            if (whiteness[k] == 0 && whiteness[k+1] == 1) {
                lineStart = k+1;
            }

            if (whiteness[k] == 1 && whiteness[k+1] == 0) {
                lineEnd = k;
            }
            lineCentre = (lineStart + lineEnd)/2;
        }

		for (int j = 0; j < 16; j++) {
            printf("%d ", whiteness[j]);
		}

		Sleep(0,100000);
		printf("\n");
		update_screen();
	}
}
