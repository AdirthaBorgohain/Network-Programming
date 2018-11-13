#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{

	int welcomeSocket, newSocket, i, k, flag=0, term1, term2, ans;
	char buffer[1024],op, *t1, *t2;

	struct sockaddr_in serverAddr;
	struct sockaddr_in serverStorage;

	socklen_t addr_size;

	/*---- Create the socket. The three arguments are: ----*/
	/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

	/*---- Configure settings of the server address struct ----*/
	/* Address family = Internet */
	serverAddr.sin_family = AF_INET;

	/* Set port number, using htons function to use proper byte order */
	serverAddr.sin_port = htons(16012);

	/* Set IP address to localhost */
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	/* Set all bits of the padding field to 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	/*---- Bind the address struct to the socket ----*/
	bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	/*---- Listen on the socket, with 5 max connection requests queued ----*/
	if (listen(welcomeSocket, 5) == 0)
		printf("Waiting for computational problem to solve: \n");
	else
		printf("Error\n");

	/*---- Accept call creates a new socket for the incoming connection ----*/
	while (1)
	{
		addr_size = sizeof serverStorage;
		newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);
		/*---- Identify clients like this. The following information of client are taken from client due to connect function ----*/
		/*---- Change of the following information at client side can not be done. However, padding field may be changed and that may be tried ----*/
		struct sockaddr_in *cliIP = (struct sockaddr_in *)&serverStorage;
		struct in_addr ipAddr = cliIP->sin_addr;

		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);

		char *ID = cliIP->sin_zero;
		char str2[8];

		inet_ntop(AF_INET, &ID, str2, 8);

		for (i = 0; i < 8; i++)
		{
			printf("%c", serverStorage.sin_zero[i]);
		}

		/*---- A  one liner ----*/
		printf("\nGot a client connection from IP, port: <%s, %d>\n", inet_ntoa(serverStorage.sin_addr), serverStorage.sin_port);


		/* ---- Receive message from client, if any ---- */
		recv(newSocket, buffer, 1024, 0);
	
		printf("\nProblem received from client <%s, %d>: %s\n", inet_ntoa(serverStorage.sin_addr), serverStorage.sin_port, buffer);

		/* ---- Parsing and computation of the problem ---- */
		i = k = 0;
		while(buffer[i] != '\0'){
			if(buffer[i] == '+' || buffer[i] == '-' || buffer[i] == '/' || buffer[i] == '*'){
				flag = 1;
				op = buffer[i++];
				t1[k] = '\0';
				k = 0;
			}
			if(flag == 0){
				t1[k++] = buffer[i];
			}
			else{
				t2[k++] = buffer[i];
			}
			i++;
		}
		t2[k] = '\0';

		term1 = atoi(t1);
		term2 = atoi(t2);

		if(op == '+'){
			ans = term1 + term2;
		} else if(op == '-'){
			ans = term1 - term2;
		} else if(op == '*'){
			ans = term1 * term2;
		} else if(op == '/'){
			ans = term1/term2;
		} 

		/*---- Convert int to char ----*/
		snprintf(buffer, 23, "%d", ans);
		sleep(3);
		/*---- Send message to the socket of the incoming connection ----*/
		send(newSocket, buffer, 23, 0);

		close(newSocket);
	}
	return 0;
}