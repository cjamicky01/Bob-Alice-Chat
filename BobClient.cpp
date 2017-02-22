// client.cpp -- client
using namespace std;
#include <iostream>
#include <sys/socket.h> // socket header files
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


#define MY_PORT      5555 // port number
#define SIZE         1024
#define MY_UNSERVED  5
#define SLEEP_TIME   4000000

/*
internet socket address structure
struct sockaddr_in {
   short int           sin_family;
   unsigned short int  sin_port;
   struct in_addr      sin_addr;
   unsigned char       sin_zero[8];
}

port and address need to be in network byte order
here are some conversion functions -- see man pages

htons() host to network short
htonl() host to network long
ntohs() network to host short
nthoh() network to host long
inet_ntoa() ip number to network address converter
*/

// ip address of the machines where server resides
#define RLC "127.0.0.1"
/*internet socket address structure
struct sockaddr_in {
   short int           sin_family;
   unsigned short int  sin_port;
   struct in_addr      sin_addr;
   unsigned char       sin_zero[8];
}
*/

int main(int argc, char * argv[]) {
   struct sockaddr_in first_addr,client_addr, second_addr;
   char buf[SIZE];
   char buf2[SIZE];
char space[SIZE];
   int child_pid;
   int listen_status;
   int accept_status;
   int bind_status;
   int rcv_count;
   int buffer_len;
   int flags;
   int i;
   socklen_t client_addr_len;
   int max_count;
   int send_status;
   int connect_status;
   int my_pid;
   int iteration;
   int first_socket;
   int second_socket;
   char *SERVICE_ADDR_STR="";
   char Name [50];
   char Friends [50];
   int portNum;
   string newAddr;
   short unsigned int newPort;

   cout << "Please Enter Your Name: ";
   cin >> Name;
   cout << "Please Enter Your Port Number: ";
   cin >> portNum;
   cout << "Please Enter Your Friends: ";
   cin >> Friends;
   cout << "Connecting..." << endl;
   /* args are socket(address family, type, protocol)
      where family is always PF_INET, type is SOCK_STREAM for connected link,
      0 lets the system choose the underlying ipc protocol
   */
   
   //find out the hostname and IP address 
   char hostname[50];
   struct hostent *entry;
   if(gethostname(hostname, sizeof(hostname))==0)
   {
    //printf("Hostname =%s\n",hostname);
    if((entry=gethostbyname(hostname))!=NULL)
    { SERVICE_ADDR_STR=(char *)calloc(16,sizeof(char));
     strcpy(SERVICE_ADDR_STR,inet_ntoa(*(struct in_addr *)entry->h_addr_list[0]));
     //printf("IP Address =%s\n",SERVICE_ADDR_STR);
     }
     }

   // create a socket
   first_socket = socket(AF_INET,SOCK_STREAM,0);

   if (first_socket<0) {
      perror("error opening the first socket:");
      exit(1);
   }

   // connect to it; use my own ip for now
   first_addr.sin_family = AF_INET;
   first_addr.sin_port = MY_PORT;
   first_addr.sin_addr.s_addr = inet_addr(RLC);
   connect_status = connect(first_socket,(struct sockaddr *) & first_addr,
      sizeof(struct sockaddr_in));
   if (connect_status<0) {
      perror("error on connecting first_socket:");
      exit(1);
   }

   // from time to time send some data
   flags = 0;
   my_pid = getpid();
   iteration = 0;
   
      // copy message into buffer
      sprintf(buf,"%s IP Address:%s Port Number:%d\n",Name, SERVICE_ADDR_STR, portNum);
      sprintf(buf2,"%s\n", Friends);
      usleep(SLEEP_TIME); // wait a while
      // send buffer though socket
      send_status = send(first_socket,buf,strlen(buf)+2,flags);
      if (send_status<0) {
         perror("error on send\n");
         exit(1);
      }
      usleep(SLEEP_TIME);
      send_status = send(first_socket,buf2,strlen(buf)+2,flags);
      if (send_status<0) {
         perror("error on send\n");
         exit(1);
      }
      cout<< "Would you like to see your friends? (yes/no)" << endl;
	cin >> space;
	cout << space << endl;
	usleep(SLEEP_TIME);
	send_status = send(first_socket,space,strlen(space)+2,flags);
      if (send_status<0) {
         perror("error on send\n");
         exit(1);
      }
      //cout << "Just sent " << send_status << " chars" << endl;
      //cout << "Message: " << buf << endl;
      //cout << "Message 2: " << buf2 << endl;
 while((rcv_count=recv(first_socket,space,SIZE,flags))<=0);
	rcv_count = 0;
 	cout << space << endl;
	




	close(first_socket);
	cout << "Please Enter the IP address to connect to: ";
	cin >> newAddr;
	cout << "Please Enter the Port Number: ";
	cin >> newPort;

// create a socket
   second_socket = socket(AF_INET,SOCK_STREAM,0);

   if (second_socket<0) {
      perror("error opening the second socket:");
      exit(1);
   }

   // connect to it; use my own ip for now
   second_addr.sin_family = AF_INET;
   second_addr.sin_port = newPort;
   second_addr.sin_addr.s_addr = inet_addr(newAddr.c_str());
   connect_status = connect(second_socket,(struct sockaddr *) & second_addr,
      sizeof(struct sockaddr_in));
   if (connect_status<0) {
      perror("error on connecting second_socket:");
      exit(1);
   }
	sprintf(buf,"Hello! How are you?");

	send_status = send(second_socket,buf,strlen(buf)+2,flags);
      if (send_status<0) {
         perror("error on send to Alice\n");
         exit(1);
      }
   
}