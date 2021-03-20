#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <algorithm>
#include <chrono>

#define PORT	 12000

using namespace std;


int main() { 
	int sockfd, n;
    socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr;
    buffer[1023] = '\0';

	// Create a UDP socket
	// Notice the use of SOCK_DGRAM for UDP packets
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr)); 
 
	// Fill server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
	servaddr.sin_port = htons(PORT); // port number
    
    //create timeval structure to timeout receiver after 1 second
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    //set socket option to allow for timeout
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof(tv)) < 0){
        perror("Error");
    }
 
    //ping 10 times  
    printf("Pinging UDP socket with 10 pings\n");
    for (int i = 0; i < 10; i++){
        
        //send message to server    
        sendto(sockfd, (const char *) buffer, sizeof(buffer), 
                MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
        
        //record start time
        auto start = chrono::steady_clock::now();

        //receive message from server
        n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 
                MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        
        //record end time
        auto end = chrono::steady_clock::now();

        if (n < 0){
            //print timeout message if no message received
            printf("Timeout from ping %d\n", i);
        }else{          
            //sleep for one second to keep constant ping speed
            //log reply and time 
           sleep(1);
           cout << "Reply from ping " << i << " in " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " ns" << endl;
        }

        //null terminate buffer
        buffer[n] = '\0';

    }
    return 0; 
}
