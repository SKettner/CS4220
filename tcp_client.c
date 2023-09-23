/*tcp_client.c*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main()
{
    
    printf("Configuring remote address...\n");
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    
    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    
    printf("Creating socket...\n");
    clientSocket = socket(PF_INET, SOCK_STREAM, 0); 
    if (!ISVALIDSOCKET(clientSocket)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(9004);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero); 

    printf("Connecting...\n");
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    
    printf("Connected.\n");
    printf("To send data, enter text followed by enter.\n");
    
    while(1) 
	{

	    fd_set reads;
	    FD_ZERO(&reads);
	    FD_SET(clientSocket, &reads);
	    
	    struct timeval timeout;
	    timeout.tv_sec = 0;
	    timeout.tv_usec = 30;

	    if (select(clientSocket+1, &reads, 0, 0, &timeout) < 0) 
        {
		        fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
		        return 1;
        }

	    if (FD_ISSET(clientSocket, &reads)) 
		{
	            char read[4096];
	            int bytes_received = recv(clientSocket, read, 4096, 0);
	            if (bytes_received < 1) 
				{
	                printf("Connection closed by peer.\n");
	                break;
	            }
	            printf("Received (%d bytes): %.s", bytes_received, bytes_received, read);
	    }
		if(FD_ISSET(0, &reads)) 
		{
            printf("Got here");
			char read[4096];
			if (!fgets(read, 4096, stdin)) break;
			printf("Sending: %s", read);
			int bytes_sent = send(clientSocket, read, strlen(read), 0);
			printf("Sent %d bytes.\n", bytes_sent);
		}
	}

    printf("Closing socket...\n");
    CLOSESOCKET(clientSocket);

    printf("Finished.\n");
    return 0;
}    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
