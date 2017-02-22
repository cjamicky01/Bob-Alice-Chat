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
#define MY_PORT_RECV 4545
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
   struct sockaddr_in first_addr,client_addr,serv_addr;
   char buf[SIZE];
   char buf2[SIZE];
   int child_pid;
   int listen_status;
   int accept_status;
   int bind_status;
   int rcv_count;
   int buffer_len;
   int flags;
   int i;
   socklen_t client_addr_len;
   socklen_t serv_addr_len;
   int max_count;
   int send_status;
   int connect_status;
   int my_pid;
   int iteration;
   int first_socket;
   int second_socket;
   int third_socket; 
   int fourth_socket;
   char *SERVICE_ADDR_STR="";
   char Name [50];
   char Friends [50];
   int portNum;

   cout << "Please Enter Your Name: ";
   cin >> Name;
   cout << "Please Enter Your Port Number: ";
   cin >> portNum;
   cout << "Please Enter Your Friends: ";
   cin >> Friends;
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
   sprintf(buf,"IP Address:%s Port Number:%d Name:%s\n",SERVICE_ADDR_STR, portNum, Name);
   sprintf(buf2,"%s\n", Friends);
   usleep(SLEEP_TIME); // wait a while
      // send buffer though socket
   send_status = send(first_socket,buf,strlen(buf)+2,flags);
   if (send_status<0) {
        perror("error on send 1\n");
        exit(1);
   }
   usleep(SLEEP_TIME);
   send_status = send(first_socket,buf2,strlen(buf)+2,flags);
   if (send_status<0) {
      perror("error on send 2\n");
      exit(1);
   }
      
      //cout << "Just sent " << send_status << " chars" << endl;
   cout << "Message: " << buf << endl;
   cout << "Message 2: " << buf2 << endl;
      
   close(first_socket);
   close(second_socket);

	cout << "Are you Ready to Reviece Messages?";
	string waitresponse;
	cin >> waitresponse;
   cout << "Setting Up Listening server." << endl;





   third_socket = socket(AF_INET,SOCK_STREAM,0);

   if (third_socket<0) {
      perror("error opening the first socket:");
      exit(1);
   }
   // bind it to a port and advertise it
   // type is inet, port is my personal number, ANY means use my current ip number
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = MY_PORT_RECV;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   bind_status = bind(third_socket,(struct sockaddr *) & serv_addr,
      sizeof(struct sockaddr_in));
   if (bind_status<0) {
      perror("error on binding first_socket:");
      exit(1);
   }

   // inform the system how many unserved connections are allowed
   listen_status = listen(third_socket,MY_UNSERVED);
   if (listen_status<0) {
      perror("error on listen for fist socket:");
      exit(1);
   }
   serv_addr_len = sizeof(serv_addr);
   // wait and accept connections
   cout<<"Waiting for connections"<<endl;
   while (1) {
      memset(&serv_addr, 0, sizeof(serv_addr));
      fourth_socket = accept(third_socket, (struct sockaddr *)&serv_addr,
      &serv_addr_len);
      if (fourth_socket<0) {
         perror("error on accept on first socket:");
         exit(1);
      }
      cout << "I accepted a connection from " << 
              inet_ntoa(serv_addr.sin_addr) << endl;

	while((rcv_count=recv(fourth_socket,buf,SIZE,flags))<=0); //Recieve AliceInfo
            rcv_count = 0;

	cout << "Message From Bob: " << buf << endl;
}	
}

