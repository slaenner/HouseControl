//
//  server.h
//
//  Created by Stefan Lænner on 08/04/14.
//
//

#ifndef socketserver_h
#define socketserver_h

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


#define SERVER_ADDRESS "localhost"
#define SERVER_PORT 9930
#define BUFLEN 1024
#define SENSORDATASIZE 16
#define PACKETDATASIZE 128

typedef enum {
    PacketTypeDataReq = 1,
    PacketTypeControlReq,
    PacketTypeDataAck,
    PacketTypeControlAck
} PacketType_e;

typedef struct {
    int cmd;
} PacketTypeControl_t;

typedef struct {
    int ReqId;
    PacketType_e PacketType;
    char Data[PACKETDATASIZE];
} UdpPacket_t;

class SocketServer
{
  public:
    void CreateSocket();
    int Bind();
    void HandlePacket();
    void Listen(char *cmd);
    void SendData();
    SocketServer();
   ~SocketServer();

  private:
    sockaddr_in my_addr;
    sockaddr_in cli_addr;
    int sockfd;
};

#endif
