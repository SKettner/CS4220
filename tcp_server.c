//#include <sys/types.h> 
//#include <sys/socket.h> 
//#include <netinet/in.h> 
//#include <netdb.h>
//#define SERVER PORT 12345 
//#define BUF SIZE 4096

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
	printf("Configuring local address...\n");
    int welcomeSocket, newSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;


    printf("Creating socket...\n");
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

            
    if (!ISVALIDSOCKET(welcomeSocket)) 
	{
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
	}	
	
	printf("Binding socket to local address...\n");
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(9004);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    /*---- Bind the address struct to the socket ----*/
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    printf("Listening...\n");
    if (listen(welcomeSocket, 5) == 0) 
	{
        printf("Listening\n");
    }
    else
    {
    printf("Error\n");        
    }

    
    fd_set master;
    FD_ZERO(&master);
    FD_SET(welcomeSocket, &master);
    SOCKET max_socket = welcomeSocket;
    
    printf("Waiting for connections...\n");

    while(1) 
	{


        /* Receiving file size */
        if(recv(welcomeSocket, buffer, BUFSIZ, 0) > 0)
        {
            printf("Got here\n");


            int file_size = atoi(buffer);
            fprintf(stdout, "\nFile size : %d\n", file_size);

            FILE *received_file = fopen("Received.txt", "w");
            
            int remain_data = file_size;
            size_t len = 0;

            while ((remain_data > 0) && ((len = recv(welcomeSocket, buffer, BUFSIZ, 0)) > 0))
            {
                    fwrite(buffer, sizeof(char), len, received_file);
                    remain_data -= len;
                    fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len, remain_data);
            }
            fclose(received_file);
        }
        wait(1);

    } //while(1)


    printf("Closing listening socket...\n");
    CLOSESOCKET(welcomeSocket);

    printf("Finished.\n");
    return 0;
}

