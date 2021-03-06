#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <thread>
#include "c/fcntl.hh"
#include "c/poll.hh"
#include "Socket.hh"
#include "SocketException.hh"

#include <iostream> // debug

Socket::Socket(int32_t domain, int32_t type, int32_t protocol)
{
     lAddr_.sa_family = domain;

     poll_.fd = std::socket(domain, type, protocol);
     
     if (poll_.fd == -1)
          throw SocketException(0, errno);
}

Socket::Socket(int32_t fd)
{
     socklen_t addrlen;
     int32_t err;

     poll_.fd = fd;

     addrlen = sizeof lAddr_;
     err = std::getsockname(poll_.fd, &lAddr_, &addrlen);

     if (err)
          throw SocketException("Socket", errno);
}

Socket::~Socket()
{
     std::close(poll_.fd);
}

int
Socket::accept()
{
     socklen_t addrlen;
     int32_t fd;

     addrlen = sizeof rAddr_;
     fd = std::accept(poll_.fd, &rAddr_, &addrlen);

     if (fd == -1)
          throw SocketException("accept", errno);
     
     return fd;
}

int
Socket::bind()
{
     int32_t bind;
     uint8_t i;
       
     bind = -1;
     i = 0;

     while (bind && i < 20) {
          bind = std::bind(poll_.fd, &lAddr_, sizeof lAddr_);

          if (bind)
               std::this_thread::sleep_for(std::chrono::seconds(6));

          i++;
     }

     if (bind)
          throw SocketException("bind", errno);

     return bind;
}

int32_t
Socket::getFd()
{
     return poll_.fd;
} 

int32_t
Socket::listen(int32_t backlog)
{
     int32_t err;

     err = std::listen(poll_.fd, backlog);

     if (err)
          throw SocketException("listen", errno);

     return err;
}

ssize_t
Socket::recv(void *buf, size_t buflen, int32_t flags)
{
     ssize_t recv;

     recv = std::recv(poll_.fd, buf, buflen, flags);

     if (recv == -1)
          throw SocketException("recv", errno);

     if (!recv)
          throw SocketException("recv", ENOLINK);

     std::cout << "Socket::recv : " << (const char *) buf; // debug

     return recv;
}

ssize_t
Socket::recvfrom(void *buf, size_t buflen, int32_t flags)
{
     ssize_t recv;
     socklen_t addrlen;
 
     addrlen = sizeof rAddr_;
     recv = std::recvfrom(poll_.fd, buf, buflen, flags, &rAddr_, &addrlen);

     if (recv == -1)
          throw SocketException("recvfrom", errno);

     std::cout << "Socket::recvfrom : " << (const char *) buf; // debug
     
     return recv;
}

int32_t
Socket::pollin(int32_t timeout)
{
     int32_t fdr;

     poll_.events = POLLIN;

     fdr = std::poll(&poll_, 1, timeout);

     if (fdr == -1)
          throw SocketException("pollin", errno);
     
     return fdr;
}

ssize_t
Socket::send(const void *buf, size_t nbyte, int32_t flags)
{
     ssize_t send;

     send = std::send(poll_.fd, buf, nbyte, flags);

     if (send == -1)
          throw SocketException("send", errno);

     std::cout << "Socket::send : " << (const char *) buf; // debug
     
     return send;
}   
