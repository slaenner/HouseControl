#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring> // memcpy

// Own headers
#include "socketserver.h"

void err(char *str)
{
  perror(str);
  exit(1);
}

SocketServer::~SocketServer()
{
  std::cout << "Deleting server instance\n";
  close(sockfd);
}

SocketServer::SocketServer()
{
  bzero(&my_addr, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(SERVER_PORT);
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

void SocketServer::CreateSocket()
{
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) {
    std::cout << "Server: socket() error!\n";
  } else {
    std::cout << "Server : Socket() successful\n";
  }
}

int SocketServer::Bind()
{
  int result = bind(sockfd, (struct sockaddr* ) &my_addr, sizeof(my_addr));
  
  if(result == 0) {
    std::cout << "Server: bind() successfull\n";
  } else {
    std::cout << "Server: bind() error!\n";
  }
  
  return result;
}

void SocketServer::ReceiveData()
{
  std::cout << "ReceiveData() called\n";

  char buf[BUFLEN];
  socklen_t slen = sizeof(cli_addr);

  std::cout << "ReceiveData() called 2\n";
    
  if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
  {
    std::cout << "Server: recvfrom() error!";
  } else {
    printf("Received packet from %s:%d\nData: %s\n\n",
         inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);
  }

  
}

int main2x(void)
{
  SocketServer* server = new SocketServer();
  
  server->CreateSocket();
  server->Bind();
  
  while(1)
  {
    std::cout << "Calling ReceiveData()\n";
    server->ReceiveData();
  }
  
  delete server;
  
  return 0;
}
