"# CS4220" 
Group:
Roman Joska
Sherman Kettner
Alex Traffis

Statement
We/I have neither given nor received unauthorized assistance on this work

How to Build/Run
sudo fuser -k 9004/tcp (run this first because it may cause issues with client/server connection)
gcc tcp_server.c -o tcp_server -lresolv (use this if you want to compile the c yourself)
gcc tcp_client.c -o tcp_client -lresolv (use this if you want to compile the c yourself)
First run ./tcp_server 
then run ./tcp_client
then look at the Revieved.txt

Resources
In the book, there are some files at the end that you brought to our attention that outline how to approach the problem. Also, Alex has made something similar in the past that dealt with multicast, but we had that former code of his to look upon and reference. As well as other resources that were provided such as the C refresher because none of us are the best at C. The resource on sockets was nice to learn about what we were making and also introduced us to the functions that we need to use for this project. We referred to and used examples of code from https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/, Van Winkle, L. (2019). Hands-on network programming with C. Pakt Publishing. Found in the O’Reilly database through the UCCS Kraemer Library, and https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/. We also used this site to help us with functions https://pubs.opengroup.org/onlinepubs/009696699/functions/accept.html#:~:text=The%20accept()%20function%20shall,file%20descriptor%20for%20that%20socket. 

Issues
We had a few issues as the code in the book was provided in such a weird format that it was hard to read and hard to implement ourselves into the project. Also, the code in the book lacked comments and descriptions so even though it was a good reference we still had to spend a lot of time reverse-engineering the code and looking up similar examples to learn what exactly it was doing and how to get it to work. Segmentation fault is the most joyous thing in C. We also took a bit to try and figure out how to run the client and server as they are two separate programs. The getting client to connect to the server was confusing. Sending a file was not easy and we troubleshot code in this section for well over 3-5 hours. We have encrypted the file. We don't know how, but we did so, and now we can't unencrypt it. We were experiencing issues where we were sending data of some sort, but we were struggling to send the file we wanted to send. We found a big issue we kept sending the file correctly but then immediately reopening the file and thus deleting what we were sent. This is what happens when you forget how while loops work.
