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
        fd_set reads;
        reads = master;
        if (select(max_socket+1, &reads, 0, 0, 0) < 0) 
		{
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }
        
        SOCKET i;
        for(i = 1; i <= max_socket; ++i) 
		{
            if (FD_ISSET(i, &reads)) 
			{

                //printf("Something Happened\n");
                //Handle socket
            }
        }
        
        
        if (i == welcomeSocket) 
		{
            struct sockaddr_storage client_address;
            socklen_t client_len = sizeof(client_address);
            SOCKET socket_client = accept(welcomeSocket,
                    (struct sockaddr*) &client_address,
                    &client_len);
            if (!ISVALIDSOCKET(socket_client)) 
			{
                fprintf(stderr, "accept() failed. (%d)\n",
                        GETSOCKETERRNO());
                return 1;
            }

            FD_SET(socket_client, &master);
            if (socket_client > max_socket)
			{
				max_socket = socket_client;
			} 

            char address_buffer[100];
            getnameinfo((struct sockaddr*)&client_address,
                    client_len,
                    address_buffer, sizeof(address_buffer), 0, 0,
                    NI_NUMERICHOST);
            printf("New connection from %s\n", address_buffer);

        } 
		
		else 
		{
	        char read[1024];
	        int bytes_received = recv(i, read, 1024, 0);
	        if (bytes_received < 1) 
			{
	            FD_CLR(i, &master);
	            CLOSESOCKET(i);
	            continue;
	        }
	
	        int j;
	        for (j = 0; j < bytes_received; ++j)
	        {
		        read[j] = toupper(read[j]);
		        send(i, read, bytes_received, 0);
			}

    	}
    	
    	          //  } //if FD_ISSET
       // } //for i to max_socket
    } //while(1)


    printf("Closing listening socket...\n");
    CLOSESOCKET(welcomeSocket);

    printf("Finished.\n");
    return 0;
}

