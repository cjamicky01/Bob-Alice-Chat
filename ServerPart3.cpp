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


#define MY_PORT      5555
#define BOB_PORT     1234
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


int main() {
	struct sockaddr_in first_addr, client_addr, third_addr;
	char BobInfo[SIZE];
	char AliceInfo[SIZE];
	char BobFriends[SIZE];
	char AliceFriends[SIZE];
	char temp[SIZE];
	char temp2[SIZE];
	char temp3[SIZE];
	int child_pid;
	int listen_status;
	int bind_status;
	int rcv_count;
	int buffer_len;
	int flags;
	int i;
	socklen_t client_addr_len, second_addr_len;
	int max_count;
	int iteration;
	int my_pid;
	int first_socket;
	int second_socket;
	int third_socket;
	int fourth_socket;
	int portNum;
	string friends;
	char * pch;
	//unsigned char space[SIZE];
	//unsigned char space2[SIZE];


	char *SERVICE_ADDR_STR = "";
	/* args are socket(address family, type, protocol)
	   where family is always AF_INET, type is SOCK_STREAM for connected link,
	   0 lets the system choose the underlying ipc protocol
	*/
	//find out the hostname and IP address 
	char hostname[50];
	struct hostent *entry;
	if (gethostname(hostname, sizeof(hostname)) == 0)
	{
		printf("Hostname =%s\n", hostname);
		if ((entry = gethostbyname(hostname)) != NULL)
		{
			SERVICE_ADDR_STR = (char *)calloc(16, sizeof(char));
			strcpy(SERVICE_ADDR_STR, inet_ntoa(*(struct in_addr *)entry->h_addr_list[0]));
			printf("IP Address =%s\n", SERVICE_ADDR_STR);
		}
	}

	// create a socket
	first_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (first_socket < 0) {
		perror("error opening the first socket:");
		exit(1);
	}
	// bind it to a port and advertise it
	// type is inet, port is my personal number, ANY means use my current ip number
	first_addr.sin_family = AF_INET;
	first_addr.sin_port = MY_PORT;
	first_addr.sin_addr.s_addr = INADDR_ANY;
	bind_status = bind(first_socket, (struct sockaddr *) & first_addr,
		sizeof(struct sockaddr_in));
	if (bind_status < 0) {
		perror("error on binding first_socket:");
		exit(1);
	}

	// inform the system how many unserved connections are allowed
	listen_status = listen(first_socket, MY_UNSERVED);
	if (listen_status < 0) {
		perror("error on listen for fist socket:");
		exit(1);
	}
	client_addr_len = sizeof(client_addr);
	// wait and accept connections
	cout << "Waiting for connections" << endl;
	//while (1) {
	memset(&client_addr, 0, sizeof(client_addr));
	second_socket = accept(first_socket, (struct sockaddr *)&client_addr,
		&client_addr_len);
	if (second_socket < 0) {
		perror("error on accept on first socket:");
		exit(1);
	}
	cout << "I accepted a connection from " <<
		inet_ntoa(client_addr.sin_addr) << endl;

	//child_pid = fork(); // fork out process to receive data from new socket
	//if (child_pid==0) {
	   //child_pid = getpid();
	flags = 0;

	// receive buffer
	while ((rcv_count = recv(second_socket, temp, SIZE, flags)) <= 0); //Recieve AliceInfo
	rcv_count = 0;
	strcpy(AliceInfo, temp);
	//usleep(400000);
	cout << "Message 1 Alice: " << temp << endl;
	//strcpy(temp3, temp);
	//usleep(400000);
	while ((rcv_count = recv(second_socket, temp2, SIZE, flags)) <= 0); //Recieve AliceFriends
	rcv_count = 0;
	strcpy(AliceFriends, temp2);
	//usleep(400000);
	cout << "Message 2 Alice: " << temp2 << endl;
	//usleep(400000);
		/*pch = strtok(temp, " ");
	cout << "PCH: " << pch << endl;
	if(strcmp(pch, "Bob")==0){
	strcpy(BobInfo, temp3);
	strcpy(BobFriends, temp2);
cout << "AliceInfo: ";
		cout << AliceInfo << endl;
		cout << "BobInfo: ";
		cout << BobInfo << endl;
		}
		if(strcmp(pch, "Alice")==0) {
		strcpy(AliceInfo, temp3);
		strcpy(AliceFriends, temp2);
		cout << "AliceInfo: ";
		cout << AliceInfo << endl;
cout << "BobInfo: ";
		cout << BobInfo << endl;
		}*/
	


	//close(first_socket);
	//close(second_socket);
	child_pid = fork();
	if (child_pid == 0) {
		child_pid = getpid();
		while (1) {

			listen_status = listen(first_socket, MY_UNSERVED);
			if (listen_status < 0) {
				perror("error on listen for third socket:");
				exit(1);
			}
			memset(&client_addr, 0, sizeof(client_addr));
			second_socket = accept(first_socket, (struct sockaddr *)&client_addr,
				&client_addr_len);
			if (second_socket < 0) {
				perror("error on accept on first socket:");
				exit(1);
			}
			cout << "I accepted a connection from " <<
				inet_ntoa(client_addr.sin_addr) << endl;
			/*
			//Bind for new connection
			third_socket = socket(AF_INET,SOCK_STREAM,0);

		   if (third_socket<0) {
			  perror("error opening the third socket:");
			  exit(1);
		   }
		   // bind it to a port and advertise it
		   // type is inet, port is my personal number, ANY means use my current ip number
		   third_addr.sin_family = AF_INET;
		   third_addr.sin_port = BOB_PORT;
		   third_addr.sin_addr.s_addr = INADDR_ANY;
		   bind_status = bind(third_socket,(struct sockaddr *) & third_addr,
			  sizeof(struct sockaddr_in));
		   if (bind_status<0) {
			  perror("error on binding third_socket:");
			  exit(1);
		   }

		   // inform the system how many unserved connections are allowed
		   listen_status = listen(third_socket,MY_UNSERVED);
		   if (listen_status<0) {
			  perror("error on listen for third socket:");
			  exit(1);
		   }
		   second_addr_len = sizeof(third_addr);
		   // wait and accept connections
		   cout<<"Waiting for connections"<<endl;

			  memset(&third_addr, 0, sizeof(third_addr));
			  fourth_socket = accept(third_socket, (struct sockaddr *)&third_addr,
			  &second_addr_len);
			  if (fourth_socket<0) {
				 perror("error on accept on fourth socket:");
				 exit(1);
			  }



		*/


			cout << "Waiting For Bob's Response..." << endl;
			while ((rcv_count = recv(fourth_socket, temp, SIZE, flags)) <= 0);
			rcv_count = 0;
			strcpy(BobInfo, temp);
			cout << "Message 1 Bob: " << temp << endl;
			while ((rcv_count = recv(fourth_socket, temp, SIZE, flags)) <= 0);
			rcv_count = 0;
			strcpy(BobFriends, temp);
			cout << "Message 2 Bob: " << temp << endl;
			// print buf
		//cout << temp;
		//strcpy(space,buf);
		//strcpy(space2,buf2);
			//cout << "Message 2: " << temp << endl;	
		//cout << "IP Address: " << inet_ntoa(client_addr.sin_addr) << endl;
		//cout << "Port: " << portNum << endl;
		//cout << "Friends: " << friends << endl;
		//space = "test";
			cout << "Waiting for yes/no response..." << endl;
			while ((rcv_count = recv(fourth_socket, temp, SIZE, flags)) <= 0);
			rcv_count = 0;
			cout << "Response: " << temp << endl;
			int send_status;
			//cout << "Enter: " << endl;
			//cin >> space;
			//string check = temp;
			//if((strcmp(temp, "yes")==0) ){
			send_status = send(fourth_socket, AliceInfo, strlen(AliceInfo) + 2, flags);
			if (send_status < 0) {
				perror("error on send\n");
				exit(1);
			}
			//}
				/*if((strcmp(temp, "yes")==0) ){
					send_status = send(second_socket,BobInfo,strlen(BobInfo)+2,flags);
					if (send_status<0) {
					 perror("error on send\n");
					 exit(1);
				  }

			}*/
		}
		//}

	}
	return 0;
}
