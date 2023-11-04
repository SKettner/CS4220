#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>


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
    int welcomeSocket, newSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;


    //this creates a socket that use IPV4(PF_INET), TCP(SOCK_STREAM),
    //and the best protocol (0)
    printf("Creating socket...\n");
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    //if this socket creation is not successful
    if (!ISVALIDSOCKET(welcomeSocket)) 
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

        /*---- Bind the address struct to the socket ----*/
        //binds welcomeSocket to serveraddress information found in the serverAddr var
        bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));



        //this waits for up to 5 connection on the welcome socket
        if (listen(welcomeSocket, 5) == 0) 
        {
            printf("Listening...\n");
        }
        else
        {
        printf("Error\n");        
        }
        
        printf("Waiting for connections...\n");

        //this stores the max number of Sockets we can have
        SOCKET max_socket = welcomeSocket;

        //this will loop for forever
        while(1) 
        {

            //this loops through all of the posible connections on the socket
            SOCKET i;
            for(i = 1; i <= max_socket; ++i) {

                //if we find  a connections
                if (i == welcomeSocket) 
                { 
                    //declare some vars for latter use                         
                    struct sockaddr_in new_addr;
                    socklen_t addr_size = sizeof(new_addr);


                    //accept an incoming connection on welcomeSocket that stores client address
                    //in new_addr and stores the size in addr_size
                    SOCKET socket_client = accept(welcomeSocket, (struct sockaddr*)&new_addr, &addr_size);

                    printf("Connected\n");

                    int expectedSeqNum = 0;
                    
                    //declare file we will be saving to
                    FILE *file = fopen("Received.txt", "w");

                    //while we have more data to recive
                    while (1) 
                    {   //take the data recived on socket_client and store in the buffer
                        int anyMoreDataRecived = recv(socket_client, buffer, 1024, 0);
                        printf("Data recived\n");
                        if (anyMoreDataRecived <= 0)
                        {
                            printf("Finished Reciving All Data\n");
                            break;
                        }

                        char* dataRecived = buffer + 10;
                        char seqNum[10];
                        strncpy(seqNum, buffer, 10);
                        for(int i = 0; i<10; i++)
                        {
                            if(seqNum[i]=='-')
                            {
                                seqNum[i] = '\0';
                            }
                        }

                        printf("dataRecived: %s\n", dataRecived);

                        printf("seqNum: %s\n", seqNum);

                        if(atoi(seqNum)==expectedSeqNum)
                        {
                            //prints data to file
                            fputs(dataRecived, file);

                            size_t dataSizeSend = send(socket_client, seqNum, 10, 0);

                            expectedSeqNum++;
                        }
                        else
                        {
                            int temp = expectedSeqNum-1;
                            char dataToSend[10];
                            sprintf(dataToSend, "%d", temp);

                            printf("Data recived wrong: %s\n", dataToSend);

                            size_t dataSizeSend = send(socket_client, dataToSend, 10, 0);

                        }

                        bzero(seqNum, 10);

                        bzero(buffer, 1024);
                    }
                    
                    //closes the file
                    fclose(file);

                }

            }

        } //while(1)


        printf("Closing listening socket...\n");
        CLOSESOCKET(welcomeSocket);

        printf("Finished.\n");
        return 0;
    }
}

