#include <cstdint>
#include <string>
#include <netinet/in.h>
#include "Socket.hh"

#ifndef INETSOCKET_HH
#define INETSOCKET_HH

class InetSocket : public Socket {
public:
     InetSocket(int32_t type, int32_t protocol);
     InetSocket(int32_t fd);

     void clearRemoteAddr();
     uint32_t getLocalAddr();
     uint32_t getLocalAddr(std::string& addr);
     uint32_t getRemoteAddr();
     uint16_t getLocalPort();
     void setLocalAddr(uint32_t addr);
     void setLocalPort(uint16_t port);
     void setRemoteAddr(uint32_t addr);

protected:
     struct sockaddr_in *remoteAddr_;
     struct sockaddr_in *localAddr_;
};

#endif
