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

//definitions used throughou the file
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
    //Vars for later use
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    
    char address_buffer[100];
    char service_buffer[100];
    

    //this creates a socket that use IPV4(PF_INET), TCP(SOCK_STREAM),
    //and the best protocol (0)
    printf("Creating socket...\n");
    clientSocket = socket(PF_INET, SOCK_STREAM, 0); 

    //if this socket creation is not successful
    if (!ISVALIDSOCKET(clientSocket)) 
    {
        printf("Failed to create Socket.\n");
    }
    else
    {
        printf("Created Socket.\n");
      

        //The following code was copied exactly from https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
        //However this seems to be setting up where and how the 
        //the client will be conecting with the server
        
        /*---- Configure settings of the server address struct ----*/
        /* Address family = Internet */
        serverAddr.sin_family = AF_INET;
        /* Set port number, using htons function to use proper byte order */
        serverAddr.sin_port = htons(9004);
        /* Set IP address to localhost */
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        /* Set all bits of the padding field to 0 */
        memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero); 

        //This is where we attempt to connect to the server 
        printf("Connecting...\n");
        addr_size = sizeof serverAddr;
        int testing = connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
        
        //If the connection succeeds 
        if(testing == 0)
        {
            printf("Connected.\n");


            //declare the file we are opening
            FILE* file = fopen("README.txt", "r");


            printf("Sending: \n", file);

            //this var will save the data we plan on sending 
            char data[1024] = {0};

            /* Sending file data */
            //this while loop runs until all the data has been pulled out of the file
            while(fgets(data, 1024, file) != NULL)
            {
                //here is where we send the data over the socket we declared earlier
                size_t dataSizeSend = send(clientSocket, data, sizeof(data), 0);

                //if the data was send successfully 
                if(dataSizeSend > 0)
                {
                    printf("Sent %d bytes.\n", sizeof(data));
                }
                else
                {
                    printf("Send failed.\n", sizeof(data));
                }

                //clear our var of currently sent data
                bzero(data, 1024);
            }

            //close the file
            fclose(file);

            printf("Closing socket...\n");
            CLOSESOCKET(clientSocket);

            printf("Finished.\n");
            return 0;
        }
        else
        {
            printf("Connection Failed.\n");
        }
    }
}    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
