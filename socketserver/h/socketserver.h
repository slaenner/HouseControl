//
//  server.h
//
//  Created by Stefan Lænner on 08/04/14.
//
//

#ifndef socketserver_h
#define socketserver_h

#define SERVER_ADDRESS "192.168.1.13"
#define SERVER_PORT 9930
#define BUFLEN 512
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
    void ReceiveData();
    void SendData();
    SocketServer();
   ~SocketServer();

  private:
    sockaddr_in my_addr;
    sockaddr_in cli_addr;
    int sockfd;
};

#endif
