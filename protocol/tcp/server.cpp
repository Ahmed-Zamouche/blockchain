#include "spdlog/spdlog.h"
#include "utils.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int accept(int fd) {

  struct sockaddr_in addr {};
  socklen_t addr_len = sizeof(addr);

  int new_fd = accept(fd, (struct sockaddr *)&addr, &addr_len);

  if (new_fd < 0) {
    spdlog::error("{}:{} accept(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    return -1;
  }

  spdlog::info("{}:{} accept(): {}:{}", utils::basename(__FILE__), __LINE__,
               inet_ntoa(addr.sin_addr), addr.sin_port);

  uint8_t buffer[1024]{};
  int nbytes = read(new_fd, buffer, sizeof(buffer));
  if (nbytes < 0) {
    spdlog::error("{}:{} read(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    close(new_fd);
    return -1;
  }

  buffer[nbytes] = '\0';
  spdlog::info("{}:{} read(): {}", utils::basename(__FILE__), __LINE__,
               (char *)buffer);

  nbytes = write(new_fd, buffer, strlen((char *)buffer));
  if (nbytes < 0) {
    spdlog::error("{}:{} write(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    close(new_fd);
    return -1;
  }
  close(new_fd);
  return 0;
}

int server(uint16_t port) {

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    spdlog::error("{}:{} socket(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    return -1;
  }

  struct sockaddr_in addr {};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    spdlog::error("{}:{} bind(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    close(fd);
    return -1;
  }
  if (listen(fd, 5) < 0) {
    spdlog::error("{}:{} listen(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    close(fd);
    return -1;
  }

  accept(fd);

  close(fd);
  return 0;
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  spdlog::info("{}:{} server()", utils::basename(__FILE__), __LINE__);
  return server(12345);
}
