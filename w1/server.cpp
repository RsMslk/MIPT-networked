#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <iostream>
#include "socket_tools.h"
#include <thread>
#include <time.h>

int init(int sfd)
{
  while (true)
  {
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(sfd, &readSet);

    timeval timeout = { 0, 100000 }; // 100 ms
    select(sfd + 1, &readSet, NULL, NULL, &timeout);


    if (FD_ISSET(sfd, &readSet))
    {
      constexpr size_t buf_size = 1000;
      static char buffer[buf_size];
      memset(buffer, 0, buf_size);

      ssize_t numBytes = recvfrom(sfd, buffer, buf_size - 1, 0, nullptr, nullptr);
      if (numBytes > 0)
      {
        printf("%s\n", buffer); // assume that buffer is a string
        return 1;
        break;
      }
    }
  }
}

void Listen(int sfd, int sfd_send, addrinfo resAddrInfo)
{
  ssize_t res;
  clock_t start, stop;
  start = clock ();
  std::string input;
  while (true)
  {
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(sfd, &readSet);

    timeval timeout = { 0, 100000 }; // 100 ms
    select(sfd + 1, &readSet, NULL, NULL, &timeout);


    if (FD_ISSET(sfd, &readSet))
    {
      constexpr size_t buf_size = 1000;
      static char buffer[buf_size];
      memset(buffer, 0, buf_size);

      ssize_t numBytes = recvfrom(sfd, buffer, buf_size - 1, 0, nullptr, nullptr);
      if (numBytes > 0)
      {
        printf("%s\n", buffer); // assume that buffer is a string
        stop = clock();
        input = std::to_string((double) (stop - start));
        res = sendto(sfd_send, input.c_str(), input.size(), 0, resAddrInfo.ai_addr, resAddrInfo.ai_addrlen);
      }
    }
  }
}

int main(int argc, const char **argv)
{
  const char *recv_port = "2023";
  const char *send_port = "2022";
  addrinfo resAddrInfo;
  int sfd = create_dgram_socket(nullptr, recv_port, nullptr);
  int sfd_send = create_dgram_socket("localhost", send_port, &resAddrInfo);

  if (sfd == -1)
  {
    printf("Cannot create a socket_recv\n");
    return 1;
  }
  if (sfd_send == -1)
  {
    printf("Cannot create a socket_send\n");
    return 1;
  }
  fd_set readSet;
  FD_ZERO(&readSet);
  FD_SET(sfd, &readSet);

  timeval timeout = { 0, 100000 }; // 100 ms
  select(sfd + 1, &readSet, NULL, NULL, &timeout);
  int initiated = 0;
  initiated = init(sfd);
  if(initiated == 1)
  {
    std::string input = "Hello";
    ssize_t res;
    res = sendto(sfd_send, input.c_str(), input.size(), 0, resAddrInfo.ai_addr, resAddrInfo.ai_addrlen);
    printf("Hello msg sent\n");
    if (res == -1)
      std::cout << strerror(errno) << std::endl;
    printf("Listening!\n");
    Listen(sfd, sfd_send, resAddrInfo);
  }


  
  return 0;
}
