#CS4220
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
The source provided by the instructor was our starting point. https://www.baeldung.com/cs/networking-go-back-n-protocol#introduction. This site gave us the basic outline and we started coding. The go-back-N wiki helped with understanding the variables in the pseudocode. The only other resources were C refershers from sites like https://www.tutorialspoint.com/ and https://www.w3schools.com/

Issues
Struggle to start as we found multiple examples doing the protocol in vastly different ways so its hard to know how to implement it. How do you check for corruption in the files. We decided to ignore checking for corruption as TCP has a built in way to check but we wern't sure if that would be working for us or not but there is also no documentation on how the corruption checking is done. C and pointers are once again a confusing topic so we needed some time and resources to understand that. Converting an int to a char to add it to the front of the file we are sending so it knows what we just sent is also annoying. Sending both ways was also a possible issue. Once we finished the first version of the code we put in print statements as shockingly we had errors and needed to fix them. A weird issue where our client/server stopped connecting to each other. Our N variable is randomly changing and we are not sure why. Segmentation fault we are officially coding in C. We also implemented code that is there to catch errors but we don't understand how to test it because we don't know how to force an error.
