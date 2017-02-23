/*******************************************************************************
Name: Connor Jamicky
Date:2/23/2017
File: BobClient
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
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


#define MY_PORT      5555 // port number
#define PORT 		4545
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
	/***************************************************************************
	This Section is declaring all the variables that will be needed in most
	of the program, along with gathering some input of the client such as name,
	desired port, and a friends list.
	***************************************************************************/
	struct sockaddr_in first_addr, client_addr, second_addr;
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
	char *SERVICE_ADDR_STR = "";
	char Name[50];
	char Friends[50];
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


	/***************************************************************************
	This Section is gathering host information and setting some socket settings.
	args are socket(address family, type, protocol)
	where family is always PF_INET, type is SOCK_STREAM for connected link,
	0 lets the system choose the underlying ipc protocol
	***************************************************************************/
	char hostname[50];															//Make a space for the hostname
	struct hostent *entry;
	if (gethostname(hostname, sizeof(hostname)) == 0)							//This gets the host name and IP and outputs them both
	{																			//	Along with setting some information for the socket 
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
	first_addr.sin_addr.s_addr = inet_addr(RLC);
	connect_status = connect(first_socket, (struct sockaddr *) & first_addr,	//Try to connect to the server
		sizeof(struct sockaddr_in));
	if (connect_status < 0) {													//Check for errors when connecting
		perror("error on connecting first_socket:");
		exit(1);
	}

	
	/***************************************************************************
	This Section is sending the messages concerning Bob's connection info and
	friends, along with returning information about his friends.
	***************************************************************************/
	flags = 0;																	//Set the flags for sending information

	sprintf(buf, "IP Address:%s Port Number:%d Name:%s\n", SERVICE_ADDR_STR, portNum, Name);//Convert input to proper format in buffer
	sprintf(buf2, "%s\n", Friends);												//Convert input to proper format in buffer
	usleep(SLEEP_TIME);															//Wait a while
						
	send_status = send(first_socket, buf, strlen(buf) + 2, flags);				//Send Bob's Connection information to the server
	if (send_status < 0) {														//Check for errors sending
		perror("error on send 1\n");
		exit(1);
	}
	usleep(SLEEP_TIME);
	send_status = send(first_socket, buf2, strlen(buf2) + 2, flags);				//Send Bob's Friends to the server
	if (send_status < 0) {														//Check for errors sending
		perror("error on send\n");
		exit(1);
	}
	usleep(SLEEP_TIME);
	cout << "Would you like to see your friends? (yes/no)" << endl;				//Ask if they want to see their friends
	cin >> space;																//Pull user input

	usleep(SLEEP_TIME);
	send_status = send(first_socket, space, strlen(space) + 2, flags);			//Send the user input to the server
	if (send_status < 0) {														//Check for errors sending		
		perror("error on send\n");
		exit(1);
	}

	while ((rcv_count = recv(first_socket, space, SIZE, flags)) <= 0);			//Wait for server to reply with friends information
	rcv_count = 0;																//reset recv cout
	cout << space << endl;														//Output the friends information

	close(first_socket);														//Close the socket to prepare for the next part


	/***************************************************************************
	This Section is getting the new connection parameters from the user, these 
	are displayed for them and input is used for confirmation. There is also 
	setup for this new connection to Alice.
	***************************************************************************/
	cout << "Please Enter the IP address to connect to: ";						//Prompt the user for the IP address
	cin >> newAddr;																//Get the input
	cout << "Please Enter the Port Number: ";									//Prompt the user for the port number
	cin >> newPort;																//Get the input

	
	second_socket = socket(AF_INET, SOCK_STREAM, 0);							//Create the new socket for connecting to Alice

	if (second_socket < 0) {													//Check for errors opening the socket
		perror("error opening the Communication socket:");
		exit(1);
	}

																				//Bind the socket to a port and Advertise it 
	second_addr.sin_family = AF_INET;											//Type is INET, port is the port specified by the user, Any means use my current IP
	second_addr.sin_port = newPort;												//Setting up the settings for the communcation socket
	second_addr.sin_addr.s_addr = inet_addr(newAddr.c_str());
	connect_status = connect(second_socket, (struct sockaddr *) & second_addr,	//Connect to Alice
		sizeof(struct sockaddr_in));
	if (connect_status < 0) {													//Check for errors connecting
		perror("error on connecting Communication socket:");
		exit(1);
	}


	/**********************************************************************
	Everything After This point is a loop of communication
	between Bob and Alice
	***********************************************************************/
	string message;															//Used for inputting messages from user
	while (1) {
		cout << "What would you like to say: ";								//Prompt user for message
		getline(cin, message);												//Gather input to send

		strcpy(buf, message.c_str());										//Copy full string into buffer
		usleep(SLEEP_TIME);													//Sleep to get more consistent behavior
		send_status = send(second_socket, buf, strlen(buf) + 2, flags);		//send the message to Alice
		if (send_status < 0) {												//Check for errors
			perror("error on send to Alice\n");
			exit(1);
		}

		while ((rcv_count = recv(second_socket, buf, SIZE, flags)) <= 0);	//Recieve the Message from Alice
		rcv_count = 0;														//Reset count
		cout << "They Replied: " << buf << endl;							//Output Reply


	}

}
