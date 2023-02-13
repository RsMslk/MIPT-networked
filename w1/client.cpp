#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <iostream>
#include "socket_tools.h"

#include <fstream>
#include <unistd.h>
#include <thread>

void Listen(int sfd)
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
        printf("%s\n", buffer); // assume that buffer is a string
    }
  }
}


int main(int argc, const char **argv)
{
  std::string input;
  std::cin >> input;
  const char *send_port = "2023";
  const char *recv_port = "2022";

  addrinfo resAddrInfo;
  int sfd = create_dgram_socket("localhost", send_port, &resAddrInfo);
  int sfd_recv = create_dgram_socket(nullptr, recv_port, nullptr);
  if (sfd == -1)
  {
    printf("Cannot create a socket_send\n");
    return 1;
  }
  if (sfd_recv == -1)
  {
    printf("Cannot create a socket_recv\n");
    return 1;
  }
  ssize_t res;
  
  if (input =="init")
  {
    std::string hello = "Hello? From client";
    res = sendto(sfd, hello.c_str(), hello.size(), 0, resAddrInfo.ai_addr, resAddrInfo.ai_addrlen);
    printf("Hello msg sent\n");
  }

  std::thread listen_thread(Listen, sfd_recv);
  listen_thread.detach();

  std::ifstream in("./send_text.txt");
  if (in.is_open())
  {
    while (std::getline(in, input))
    {
      printf(">");
      res = sendto(sfd, input.c_str(), input.size(), 0, resAddrInfo.ai_addr, resAddrInfo.ai_addrlen);
      sleep(10);
      if (res == -1)
        std::cout << strerror(errno) << std::endl;
    }
  }
  else  
  {
    printf("Can't open input file");
  }
  in.close();
  
  return 0;
}
