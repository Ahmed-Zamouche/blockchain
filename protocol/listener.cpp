#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#include <Windows.h>
#include <Winsock.h> // before Windows.h, else Winsock 1 conflict
typedef int socklen_t;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

#include "spdlog/spdlog.h"
// #include <filesystem>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace fs = std::filesystem;

#define MSGBUFSIZE 256

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Command line args should be multicast group and port\n");
    printf("(e.g. for SSDP, `listener 239.255.255.250 1900`)\n");
    return 1;
  }

  char *group = argv[1];    // e.g. 239.255.255.250 for SSDP
  int port = atoi(argv[2]); // 0 if error, which is an invalid port

#ifdef _WIN32
  //
  // Initialize Windows Socket API with given VERSION.
  //
  WSADATA wsaData;
  if (WSAStartup(0x0101, &wsaData)) {
    spdlog::error("{}:{} WSAStartup(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    return 1;
  }
#endif

  // create what looks like an ordinary UDP socket
  //
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    spdlog::error("{}:{} socket(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    return 1;
  }

  // allow multiple sockets to use the same PORT number
  //
  u_int yes = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes)) < 0) {
    spdlog::error("{}:{} setsockopt(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    return 1;
  }

  // set up destination address
  //
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
  addr.sin_port = htons(port);

  // bind to receive address
  //
  if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    spdlog::error("{}:{} bind(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    return 1;
  }

  // use setsockopt() to request that the kernel join a multicast group
  //
#if 0
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr(group);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq,
                 sizeof(mreq)) < 0) {
    spdlog::error("{}:{} setsockopt(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    return 1;
  }
#else
  (void)group;
#endif

  // now just enter a read-print loop
  //
  while (1) {
    char msgbuf[MSGBUFSIZE];
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t addrlen = sizeof(addr);
    int nbytes =
        recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *)&addr, &addrlen);
    if (nbytes < 0) {
      spdlog::error("{}:{} recvfrom(): {}", utils::basename(__FILE__), __LINE__,
                    strerror(errno));
      return 1;
    }
    msgbuf[nbytes] = '\0';
    spdlog::info("{}:{} recvfrom(): {}: {}", utils::basename(__FILE__),
                 __LINE__, inet_ntoa(addr.sin_addr), msgbuf);
  }

#ifdef _WIN32
  //
  // Program never actually gets here due to infinite loop that has to be
  // canceled, but since people on the internet wind up using examples
  // they find at random in their own code it's good to show what shutting
  // down cleanly would look like.
  //
  WSACleanup();
#endif

  return 0;
}
