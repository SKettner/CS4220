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
#include <time.h>

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

            fseek(file, 0L, SEEK_END);

            long int fileSize = ftell(file);

            fseek(file, 0, SEEK_SET);


            printf("Sending: \n");

            //this var will save the data we plan on sending 
            char data[fileSize];

            fread(data, 1, fileSize, file);

            char* currentData = data;
            char* lastAckData = data;

            int N = fileSize/1024;

            //close the file
            fclose(file);

            int sendBase = 0;
            int nextSeqNum = 0;

            clock_t startTime = clock();

           

            while(1)
            {
                printf("Got Here: \n");

                while(nextSeqNum<(sendBase+N))
                {
                    printf("and Here: \n");

                    char currentSeqNum[10];
                    sprintf(currentSeqNum, "%d", nextSeqNum);
                    strcat(currentSeqNum,"---------");
                    printf("currentSeqNum: %s\n", currentSeqNum);

                    char dataToSend[1024];
                    strcat(dataToSend, currentSeqNum);

                    char temp[1013];
                    strncpy(temp, currentData, 1013);

                    strcat(dataToSend, temp);
                    dataToSend[1024] = '\0';
                    printf("dataToSend2: %s\n", dataToSend);

                    size_t dataSizeSend = send(clientSocket, dataToSend, 1024, 0);
                    currentData += 1013;
                    nextSeqNum++;

                    printf("nextSeqNum: %d\n", nextSeqNum);
                    printf("sendBase: %d\n", sendBase);
                    printf("N: %d\n", N);

                    bzero(currentSeqNum, 10);
                    bzero(temp, 1013);
                    bzero(dataToSend, 1024);
                }

                int anyMoreDataRecived = recv(clientSocket, buffer, 1024, 0);
                printf("stuff: \n");

                if(anyMoreDataRecived>0)
                {
                    printf("Then Here: \n");

                    printf("sendBase: %d\n", sendBase);
                    printf("N: %d\n", N);

                    sendBase = 1 + atoi(buffer);
                    
                    if(sendBase >= N)
                    {
                        break;
                    }
                    if(sendBase == nextSeqNum)
                    {
                        startTime = clock();
                    }
                }
            
                if((clock() - startTime) > 15000)
                {
                    printf("Sutff: \n");

                    startTime = clock();

                    char * temp = lastAckData;
                    int temp2 = sendBase;

                    while(sendBase<=nextSeqNum)
                    {
                        char currentSeqNum[10];
                        sprintf(currentSeqNum, "%d", sendBase);
                        strcat(currentSeqNum,"---------");
                        printf("currentSeqNum: %s\n", currentSeqNum);

                        char dataToSend[1024];
                        strcat(dataToSend, currentSeqNum);

                        char temp[1013];
                        strncpy(temp, lastAckData, 1013);

                        strcat(dataToSend, temp);
                        dataToSend[1024] = '\0';
                        printf("dataToSend2: %s\n", dataToSend);

                        size_t dataSizeSend = send(clientSocket, dataToSend, 1024, 0);
                        currentData += 1013;
                        nextSeqNum++;

                        printf("nextSeqNum: %d\n", nextSeqNum);
                        printf("sendBase: %d\n", sendBase);
                        printf("N: %d\n", N);

                        bzero(currentSeqNum, 10);
                        bzero(temp, 1013);
                        bzero(dataToSend, 1024);
                    }

                    lastAckData = temp;
                    sendBase = temp2;

                }

                bzero(buffer, 10);

            }

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
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
