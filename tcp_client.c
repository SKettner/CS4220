/*tcp_client.c*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


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
    
    FILE* file = fopen("readme.txt", "r");

    printf("Sending: %s", read);
    /* Sending file data */
    while(1)
    {
        size_t temp = send(clientSocket, &file, sizeof(file), 0);

        printf("Did sending work?  %d \n", temp);

        printf("Sent %d bytes.\n", sizeof(file));
    }

    printf("Closing socket...\n");
    CLOSESOCKET(clientSocket);

    printf("Finished.\n");
    return 0;
}    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
