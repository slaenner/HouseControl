#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <string.h>


#include "socketserver.h"
#include "logger.h"

void err(char *str)
{
  perror(str);
  exit(1);
}

SocketServer::~SocketServer()
{
    RL_PRINT("Deleting server instance\n");
    close(sockfd);
}

SocketServer::SocketServer()
{
    RL_PRINT("Creating server instance\n");
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(SERVER_PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

void SocketServer::CreateSocket()
{
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        RL_PRINT("Create socket error!\n");
    }
    else
    {
        RL_PRINT("Socket created successfully\n");
    }
}

int SocketServer::Bind()
{
    int result = bind(sockfd, (struct sockaddr* ) &my_addr, sizeof(my_addr));
  
    if(result == 0)
    {
        RL_PRINT("Success\n");
    }
    else
    {
        RL_PRINT("Error!\n");
    }
  
    return result;
}

void SocketServer::Listen(char *cmd)
{
    RL_PRINT("Listening for data on socket\n");

    char data[BUFLEN];
    socklen_t slen = sizeof(cli_addr);
    
    if (recvfrom(sockfd, data, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
    {
        RL_PRINT("Error!");
    }
    else
    {
        char printBuf[100];
        sprintf(printBuf, "Received packet from %s:%d\n",
                 inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        RL_PRINT(printBuf);
        sprintf(printBuf, "Packet data contents: %s\n", data);
        RL_PRINT(printBuf);
    }
    
    /* Copy the received string to the cmd pointer */
    strcpy(cmd, data);
}

