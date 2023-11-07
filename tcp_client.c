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
    struct timeval tv;
    tv.tv_sec = 15; 
    
    

    //this creates a socket that use IPV4(PF_INET), TCP(SOCK_STREAM),
    //and the best protocol (0)
    printf("Creating socket...\n");
    clientSocket = socket(PF_INET, SOCK_STREAM, 0); 

    setsockopt(clientSocket, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv, sizeof(struct timeval));

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

            //takes us to end of file
            fseek(file, 0L, SEEK_END);

            //finds length of file
            long int fileSize = ftell(file);

            //takes us to beginning of file
            fseek(file, 0, SEEK_SET);

            printf("Sending: \n");

            //this var will save the data we plan on sending 
            char data[fileSize];

            //read in all data from file
            fread(data, 1, fileSize, file);

            //two pointers we will use to keep track of postions of data we are sending in the fi;e
            char* currentData = data;
            char* lastAckData = data;

            //this figures out how many packages we have to send
            int N = fileSize/1024;

            //close the file
            fclose(file);


            //this keeps track of how many packages we have sent since reciving a reply
            int sendBase = 0;
            int nextSeqNum = 0;


            //keeps track of time since packets sent
            clock_t startTime = clock();

           
            //as long as we have stuff to send and have not recived reply that it was recived
            while(1)
            {
                
                //for every packet that has not yet been sent and recived reply for
                while(nextSeqNum<(sendBase+N))
                {
                    //this will be the part of the packet the dictates it order 
                    char currentSeqNum[10];
                    sprintf(currentSeqNum, "%d", nextSeqNum);
                    strcat(currentSeqNum,"---------");

                    //this will store all data being sent in the packet
                    char dataToSend[1024];
                    strcat(dataToSend, currentSeqNum);

                    //this will store the data from the file to send in the packet
                    char temp[1013];
                    strncpy(temp, currentData, 1013);

                    //this will combine all data into packet
                    strcat(dataToSend, temp);
                    dataToSend[1024] = '\0';


                    //send all data in current packet it is sent dataToSend over clientSocket
                    //size is 1024 bytes
                    size_t dataSizeSend = send(clientSocket, dataToSend, 1024, 0);

                    //this tells us the next data and packet to send 
                    currentData += 1013;
                    nextSeqNum++;

                    //this clears out all arrays
                    bzero(currentSeqNum, 10);
                    bzero(temp, 1013);
                    bzero(dataToSend, 1024);
                }

                //waits for reply of server for 15 secs
                int anyMoreDataRecived = recv(clientSocket, buffer, 1024, 0);

                //if we get a reply
                if(anyMoreDataRecived>0)
                {
                    //this finds the current packet that is not know to have been recived
                    sendBase = 1 + atoi(buffer);
                    lastAckData += (int)(sendBase*1013);
                    
                    //if we have no more packets to send
                    if(sendBase >= N)
                    {
                        //end the loop as this means we were succsessful
                        break;
                    }
                    //we that the next packet needed is the next one we are going to send
                    if(!(sendBase == nextSeqNum))
                    {
                        //reset timer since this is what should happen
                        startTime = clock();

                    }
                }
            
                //if 15 seconds have passed without a reply, or the reply was wrong
                //and 15 seconds have passed 
                if((clock() - startTime) > 15000)
                {
                    //reset the timer
                    startTime = clock();

                    //this will store data and return it incase these packets also fail
                    char * temp = lastAckData;
                    int temp2 = sendBase;

                    //for every packet we need to send
                    while(sendBase<=nextSeqNum)
                    {

                         //this will be the part of the packet the dictates it order 
                        char currentSeqNum[10];
                        sprintf(currentSeqNum, "%d", sendBase);
                        strcat(currentSeqNum,"---------");

                        //this will store all data being sent in the packet
                        char dataToSend[1024];
                        strcat(dataToSend, currentSeqNum);

                        //this will store the data from the file to send in the packet
                        char temp[1013];
                        strncpy(temp, lastAckData, 1013);


                        //this will combine all data into packet
                        strcat(dataToSend, temp);
                        dataToSend[1024] = '\0';


                        //send all data in current packet it is sent dataToSend over clientSocket
                        //size is 1024 bytes
                        size_t dataSizeSend = send(clientSocket, dataToSend, 1024, 0);

                        //this tells us the next data and packet to send 
                        currentData += 1013;
                        nextSeqNum++;

                          //this clears out all arrays
                        bzero(currentSeqNum, 10);
                        bzero(temp, 1013);
                        bzero(dataToSend, 1024);
                    }

                    //reset all data back to last know recived 
                    lastAckData = temp;
                    sendBase = temp2;

                }

                //clear buffer
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
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
