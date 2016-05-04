# include <stdio.h>
# include <time.h>

//these load specific methods from the ENGR101 library
extern "C" int init(int d_lev);
extern "C" int Sleep( int sec , int usec );
extern "C" int connect_to_server(char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

int main (){

   // This sets up the RPi hardware and ensures
   // everything is working correctly
   // Value of 1 means debugging information is displayed
   init(1);

   //connects to server with the ip address 192.168.1.2
   connect_to_server("130.195.6.196", 1024);
   //sends a message to the connected server
   send_to_server("Please");
   //receives message from the connected server
   char message[24];
   receive_from_server(message);
   send_to_server(message);

return 0;
}
