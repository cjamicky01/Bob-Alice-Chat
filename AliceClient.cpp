/*******************************************************************************
Name: Connor Jamicky
Date:2/23/2017
File: AliceClient
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
#define SIZE         1024
#define MY_UNSERVED  5
#define SLEEP_TIME   4000000
#define RLC "127.0.0.1"

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
	struct sockaddr_in first_addr, client_addr, serv_addr;
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
	char *SERVICE_ADDR_STR = "";
	char Name[50];
	char Friends[50];
	int portNum;

	cout << "Please Enter Your Name: ";
	cin >> Name;
	cout << "Please Enter Your Port Number: ";
	cin >> portNum;
	cout << "Please Enter Your Friends: ";
	cin >> Friends;
	

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
	This Section is sending the messages concerning Alice's connection info and
	friends, along with preparing some for the next section.
	***************************************************************************/
	flags = 0;																	//Set the flags for sending information

	sprintf(buf, "IP Address:%s Port Number:%d Name:%s\n", SERVICE_ADDR_STR, portNum, Name);//Convert input to proper format in buffer
	sprintf(buf2, "%s\n", Friends);												//Convert input to proper format in buffer
	usleep(SLEEP_TIME);															//Wait a while
						
	send_status = send(first_socket, buf, strlen(buf) + 2, flags);				//Send Alice's Connection information to the server
	if (send_status < 0) {														//Check for errors sending
		perror("error on send 1\n");
		exit(1);
	}

	//usleep(SLEEP_TIME);														//Sleep (Might not be necessary)

	send_status = send(first_socket, buf2, strlen(buf2) + 2, flags);			//Send Alice's Friends to the server
	if (send_status < 0) {														//Check for errors sending
		perror("error on send 2\n");
		exit(1);
	}

	//cout << "Message: " << buf << endl;										//Output sent messages for verification
	//cout << "Message 2: " << buf2 << endl;

	close(first_socket);														//Close the previous sockets to prevent conflicts
	close(second_socket);


	/***************************************************************************
	This Section is setting up the socket to listen for Bob to initiate the
	communication between the two parties. In this we use the port we declared
	above. Most of this code is the same that was used previously but declared
	again for the new socket.
	***************************************************************************/
	string waitresponse;
	cout << "Are you Ready to Reviece Messages?";
	cin >> waitresponse;														//This is to allow the user to declare when they are ready
	if (waitresponse == "no") {													//Exit if they say no
		return 0;
	}
	cout << "Setting Up Listening server." << endl;								//Alert user that Listening is starting

	third_socket = socket(AF_INET, SOCK_STREAM, 0);								//Create the Socket

	if (third_socket < 0) {														//Check for errors opening the socket
		perror("error opening the listening socket:");
		exit(1);
	}
																				//Bind The socket to a port and Advertise it
																				//Type is INET, port is the port entered previously, ANY means use my current IP number
	serv_addr.sin_family = AF_INET;												//Setting up the settings for the listening socket
	serv_addr.sin_port = portNum;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bind_status = bind(third_socket, (struct sockaddr *) & serv_addr,			//Bind the socket
		sizeof(struct sockaddr_in));
	if (bind_status < 0) {														//Check for errors binding the socket
		perror("error on binding listening socket:");
		exit(1);
	}

																				//Inform the system how many unserved connections are allowed
	listen_status = listen(third_socket, MY_UNSERVED);							//Set socket to listen
	if (listen_status < 0) {													//check for errors when listening
		perror("error on listen for listening socket:");
		exit(1);
	}
	serv_addr_len = sizeof(serv_addr);						
																				
	cout << "Waiting for connections" << endl;									//Tell user we are just waiting for the other to connect
	while (1) {
		memset(&serv_addr, 0, sizeof(serv_addr));
		fourth_socket = accept(third_socket, (struct sockaddr *)&serv_addr,		//Accept the user trying to connect		
			&serv_addr_len);
		if (fourth_socket < 0) {												//Check for errors in accepting the new socket
			perror("error on accept on listening socket:");
			exit(1);
		}
		cout << "I accepted a connection from " <<								//Print where the connection was recieved from
			inet_ntoa(serv_addr.sin_addr) << endl;


		/**********************************************************************
		Everything After This point is a loop of communication 
		between Bob and Alice
		***********************************************************************/
		string message;															//Used for inputting messages from user
		while (1) {
			while ((rcv_count = recv(fourth_socket, buf, SIZE, flags)) <= 0);	//Recieve Message from Bob
			rcv_count = 0;														//Reset count

			cout << "They says: " << buf << endl;								//Output Sent Message

			cout << "What would you like to say: ";								//Gather input to send
			getline(cin, message);

			strcpy(buf, message.c_str());										//Copy full string into buffer
			usleep(SLEEP_TIME);													//Sleep to get more consistent behavior
			send_status = send(fourth_socket, buf, strlen(buf) + 2, flags);		//Send message to Bob
			if (send_status < 0) {												//Check for errors in sending
				perror("error on sending \n");
				exit(1);
			}
		}
	}
}
