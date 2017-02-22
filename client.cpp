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
#define RLC "54.245.38.243"


/*internet socket address structure
struct sockaddr_in {
   short int           sin_family;
   unsigned short int  sin_port;
   struct in_addr      sin_addr;
   unsigned char       sin_zero[8];
}
*/

int main(int argc, char * argv[]) {
   struct sockaddr_in first_addr,client_addr;
   char buf[SIZE];
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
   char *SERVICE_ADDR_STR="";
   /* args are socket(address family, type, protocol)
      where family is always PF_INET, type is SOCK_STREAM for connected link,
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
   while (1) {
      // copy message into buffer
      sprintf(buf,"pid %d message %d\n",my_pid,iteration++);
      usleep(SLEEP_TIME); // wait a while
      // send buffer though socket
      send_status = send(first_socket,buf,strlen(buf)+2,flags);
      if (send_status<0) {
         perror("error on send\n");
         exit(1);
      }
      cout << "Just sent " << send_status << " chars" << endl;
   }
}
