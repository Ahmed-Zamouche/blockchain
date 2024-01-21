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
#include <unistd.h> // for sleep()
#endif

#include "spdlog/spdlog.h"
// #include <filesystem>
#include "blockchain/common/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Command line args should be multicast group and port\n");
    printf("(e.g. for SSDP, `sender 239.255.255.250 1900`)\n");
    return 1;
  }

  char *group = argv[1];    // e.g. 239.255.255.250 for SSDP
  int port = atoi(argv[2]); // 0 if error, which is an invalid port

  // !!! If test requires, make these configurable via args
  //
  const int delay_secs = 1;
  const char *message = "Hello, World!";

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

  // set up destination address
  //
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(group);
  addr.sin_port = htons(port);

  // now just sendto() our destination!
  //
  while (1) {
    int nbytes = sendto(fd, message, strlen(message), 0,
                        (struct sockaddr *)&addr, sizeof(addr));
    if (nbytes < 0) {
      spdlog::error("{}:{} sendto(): {}", utils::basename(__FILE__), __LINE__,
                    strerror(errno));
      return 1;
    }
    spdlog::info("{}:{} sendto(): {}", utils::basename(__FILE__), __LINE__,
                 message);
#ifdef _WIN32
    Sleep(delay_secs * 1000); // Windows Sleep is milliseconds
#else
    sleep(delay_secs); // Unix sleep is seconds
#endif
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
