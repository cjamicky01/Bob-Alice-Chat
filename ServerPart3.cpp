/*******************************************************************************
Name: Connor Jamicky
Date:2/23/2017
File: ServerPart3
Project: A chat between two parties using a server for connection information
and sockets as the method of communication
*******************************************************************************/
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
	/***************************************************************************
	This Section is declaring all the variables that will be needed in most
	of the program.
	***************************************************************************/
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
	int send_status = 0;
	char *SERVICE_ADDR_STR = "";

	/***************************************************************************
	This Section is gathering host information and setting some socket settings
	and waiting for a connection from Alice.
	args are socket(address family, type, protocol)
	where family is always PF_INET, type is SOCK_STREAM for connected link,
	0 lets the system choose the underlying ipc protocol
	***************************************************************************/
	char hostname[50];							//Make a space for the hostname
	struct hostent *entry;
	if (gethostname(hostname, sizeof(hostname)) == 0)			//This gets the host name and IP and outputs them both
{										//	Along with setting some information for the socket 
		printf("Hostname =%s\n", hostname);
		if ((entry = gethostbyname(hostname)) != NULL)
		{
			SERVICE_ADDR_STR = (char *)calloc(16, sizeof(char));
			strcpy(SERVICE_ADDR_STR, inet_ntoa(*(struct in_addr *)entry->h_addr_list[0]));
			printf("IP Address =%s\n", SERVICE_ADDR_STR);
		}
	}

	first_socket = socket(AF_INET, SOCK_STREAM, 0);								//Create the socket
	if (first_socket < 0) {														//Check for errors when opening the socket
		perror("error opening the first socket:");
		exit(1);
	}
																				//Bind The socket to a port and Advertise it
	first_addr.sin_family = AF_INET;											//Type is INET, port is the port entered previously, ANY means use my current IP number
	first_addr.sin_port = MY_PORT;
	first_addr.sin_addr.s_addr = INADDR_ANY;
	bind_status = bind(first_socket, (struct sockaddr *) & first_addr,			//Try to connect to the server
		sizeof(struct sockaddr_in));
	if (bind_status < 0) {														//Check for errors when binding
		perror("error on binding first_socket:");
		exit(1);
	}

	listen_status = listen(first_socket, MY_UNSERVED);							//Set socket to listen
	if (listen_status < 0) {													//Check for errors when listening
		perror("error on listen for first socket:");
		exit(1);
	}
	client_addr_len = sizeof(client_addr);

	cout << "Waiting for connections" << endl;									//Tell user we are just waiting for the other to connect
	while (1) {
		memset(&client_addr, 0, sizeof(client_addr));
		second_socket = accept(first_socket, (struct sockaddr *)&client_addr,	//Accept the user trying to connect		
			&client_addr_len);
		if (second_socket < 0) {												//Check for errors in accepting the new socket
			perror("error on accept on first socket:");
			exit(1);
		}
		cout << "I accepted a connection from " <<								//Print where the connection was recieved from
			inet_ntoa(client_addr.sin_addr) << endl;


	/***************************************************************************
	This Section is recieveing information first from Alice, then from Bob and 
	replying to Bob with Alice's information
	***************************************************************************/
	flags = 0;																	//Set the flags for recieving information


	while ((rcv_count = recv(second_socket, temp, SIZE, flags)) <= 0);			//Recieve AliceInfo
	rcv_count = 0;																//Reset count
	strcpy(AliceInfo, temp);													//Copy message into AliceInfo
	cout << "Message 1 Alice: " << temp << endl;								//Output message for verification on server side
	
	while ((rcv_count = recv(second_socket, temp2, SIZE, flags)) <= 0);			//Recieve AliceFriends
	rcv_count = 0;																//Reset Count
	strcpy(AliceFriends, temp2);												//Copy message into Alice Friends
	cout << "Message 2 Alice: " << temp2 << endl;								//Output message for verification on server side

	listen_status = -1;															//Reset listening and socket statuses
	second_socket = -1;
cout<<"Listening"<<endl;
usleep(4000);
	listen_status = listen(first_socket, MY_UNSERVED);							//Start listening on Socket for second connection, Bob
	if (listen_status < 0) {													//Check for errors when listening
		perror("error on listen for third socket:");
		exit(1);
	}
	memset(&client_addr, 0, sizeof(client_addr));								
	second_socket = accept(first_socket, (struct sockaddr *)&client_addr,		//Accept the user trying to connect
		&client_addr_len);
	if (second_socket < 0) {													//Check for errors
		perror("error on accept on second socket:");
		exit(1);
	}
	cout << "I accepted a connection from " <<									//Print where the connection was recieved from
		inet_ntoa(client_addr.sin_addr) << endl;

	

	cout << "Waiting For Bob's Response..." << endl;							//Wait for Bob
	while ((rcv_count = recv(second_socket, temp, SIZE, flags)) <= 0);			//Recieve Bob's information
	rcv_count = 0;																//Reset count
	strcpy(BobInfo, temp);														//Copy full string
	cout << "Message 1 Bob: " << temp << endl;									//Print message for verification on server side

	while ((rcv_count = recv(second_socket, temp, SIZE, flags)) <= 0);			//Recieve Bob's friends
	rcv_count = 0;																//Reset count
	strcpy(BobFriends, temp);													//Copy full string
	cout << "Message 2 Bob: " << temp << endl;									//Print message for verification on server side

	cout << "Waiting for yes/no response..." << endl;							//Wait for Bob's Response

	while ((rcv_count = recv(second_socket, temp, SIZE, flags)) <= 0);			//Recieve response
	rcv_count = 0;																//Reset Count
	cout << "Response: " << temp << endl;										//Ouput response for verification on server side
send_status = send(second_socket, AliceInfo, strlen(AliceInfo) + 2, flags);	//Send Alice's info to Bob
	if (send_status < 0) {														//Check for errors
		perror("error on send\n");
		exit(1);
	}

	return 0;
}
}
