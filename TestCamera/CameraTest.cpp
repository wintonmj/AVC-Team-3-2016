#include <stdio.h>
#include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);

int main(){
    //This sets up the RPi hardware and ensures
    //everything is working correctly
    init(1);
    char c;
    while(true){
       //Take picture with camera
       take_picture();
       //get pixel "whit eness" from centre of image (160x120)
       c = get_pixel(160,120,3);
       //Prints read pixel value 
       printf("%d\n",c); 
       //Waits for 0.5 seconds (500000 microseconds)
       Sleep(0,500000);
       } 
return 0;}