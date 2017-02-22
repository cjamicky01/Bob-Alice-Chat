// server.cpp -- server
using namespace std;
#include <iostream>
#include <sys/socket.h> // socket header files
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define MY_PORT       5555
#define SIZE         1024
#define MY_UNSERVED  5

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

int main(int argc, char * argv[]) {
   struct sockaddr_in first_addr,client_addr;
   unsigned char buf[SIZE];
   int child_pid;
   int listen_status;
   int bind_status;
   int rcv_count;
   int buffer_len;
   int flags;
   int i;
   socklen_t client_addr_len;
   int max_count;
   int iteration;
   int my_pid;
   int first_socket;
   int second_socket;

    char *SERVICE_ADDR_STR="";
   /* args are socket(address family, type, protocol)
      where family is always AF_INET, type is SOCK_STREAM for connected link,
      0 lets the system choose the underlying ipc protocol
   */
   //find out the hostname and IP address 
   char hostname[50];
   struct hostent *entry;
   if(gethostname(hostname, sizeof(hostname))==0)
   {
    printf("Hostname =%s\n",hostname);
    if((entry=gethostbyname(hostname))!=NULL)
    { SERVICE_ADDR_STR=(char *)calloc(16,sizeof(char));
     strcpy(SERVICE_ADDR_STR,inet_ntoa(*(struct in_addr *)entry->h_addr_list[0]));
     printf("IP Address =%s\n",SERVICE_ADDR_STR);
     }
     }

   // create a socket
   first_socket = socket(AF_INET,SOCK_STREAM,0);

   if (first_socket<0) {
      perror("error opening the first socket:");
      exit(1);
   }
   // bind it to a port and advertise it
   // type is inet, port is my personal number, ANY means use my current ip number
   first_addr.sin_family = AF_INET;
   first_addr.sin_port = MY_PORT;
   first_addr.sin_addr.s_addr = INADDR_ANY;
   bind_status = bind(first_socket,(struct sockaddr *) & first_addr,
      sizeof(struct sockaddr_in));
   if (bind_status<0) {
      perror("error on binding first_socket:");
      exit(1);
   }

   // inform the system how many unserved connections are allowed
   listen_status = listen(first_socket,MY_UNSERVED);
   if (listen_status<0) {
      perror("error on listen for fist socket:");
      exit(1);
   }
   client_addr_len = sizeof(client_addr);
   // wait and accept connections
   cout<<"Waiting for connections£¡"<<endl;
   while (1) {
      memset(&client_addr, 0, sizeof(client_addr));
      second_socket = accept(first_socket, (struct sockaddr *)&client_addr,
      &client_addr_len);
      if (second_socket<0) {
         perror("error on accept on first socket:");
         exit(1);
      }
      cout << "I accepted a connection from " << 
              inet_ntoa(client_addr.sin_addr) << endl;

      child_pid = fork(); // fork out process to receive data from new socket
      if (child_pid==0) {
         child_pid = getpid();
         flags = 0;
         while (1) {
            // receive buffer
            while((rcv_count=recv(second_socket,buf,SIZE,flags))<=0);
            // print buffer
            cout << "child " << child_pid << " received " << rcv_count 
                 << " chars "  << buf << endl;
	    cout << "Message: " << buf << endl;
         }
      }
   }
} 
