#include "spdlog/spdlog.h"
#include "utils.h"
#include <arpa/inet.h>
#include <limits>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct tcp_iostream : virtual iomembuf, std::iostream {
  tcp_iostream(const char *host, uint16_t port, const char *base, size_t size)
      : iomembuf(base, size),
        std::iostream(static_cast<std::streambuf *>(this)), _port{port} {
    strncpy((char *)_host, host, sizeof(_host));
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0) {
      throw std::runtime_error(strerror(errno));
    }
    struct sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(_host);
    addr.sin_port = htons(_port);

    if (::connect(_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      close(_fd);
      throw std::runtime_error(strerror(errno));
    }
  }
  virtual ~tcp_iostream() {
    if (_fd >= 0)
      close(_fd);
    _fd = -1;
  }

  std::streamsize gcount() const { return _gcount; }

  tcp_iostream &read(char *s, std::streamsize count) {
    int nbytes = ::read(_fd, &s, count);
    if (nbytes < 0) {
      setstate(std::ios_base::failbit);
      setstate(std::ios_base::eofbit);
      _gcount = 0;
    } else {
      _gcount = nbytes;
    }
    return *this;
  }

  int get() {
    char ch;
    (void)read(&ch, 1);

    return (_gcount == 1) ? (int)ch : EOF;
  }

  tcp_iostream &get(char &ch) {
    int i = get();
    if (i != EOF) {
      ch = i;
    }
    return *this;
  }
  // tcp_stream &get(char_type *s, std::streamsize count);
  // tcp_stream &get(char_type *s, std::streamsize count, char_type delim);
  // tcp_stream &get(basic_streambuf &strbuf);
  // tcp_stream &get(basic_streambuf &strbuf, char_type delim);

  tcp_iostream &write(const char *s, std::streamsize count) {

    int nbytes = ::write(_fd, s, count);
    if (nbytes != count) {
      setstate(std::ios_base::badbit);
    }
    return *this;
  }

  tcp_iostream &put(char ch) { return write(&ch, 1); }

  iomembuf *rdbuf() const { return nullptr; }

  iomembuf *rdbuf(iomembuf *) {
    clear();
    return nullptr;
  }

  int _fd{-1};
  uint16_t _port;
  std::streamsize _gcount{std::numeric_limits<std::streamsize>::max()};
  const char _host[16] = {"127.000.000.001"};
};

namespace {

int client(const char *host, uint16_t port) {
  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    spdlog::error("{}:{} socket(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    return -1;
  }
  struct sockaddr_in addr {};
#if 0
  struct hostent *host;
  host = gethostbyname("localhost");
  if (host == NULL) {
    spdlog::error("{}:{} gethostbyname(): no such host",
        utils::basename(__FILE__), __LINE__);
    return -1;
  }
  // memcpy(host->h_addr, &addr.sin_addr.s_addr, host->h_length);
#endif

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(host);
  addr.sin_port = htons(port);

  if (::connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    spdlog::error("{}:{} connect(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    close(fd);
    return -1;
  }

  spdlog::info("{}:{} connect(): {}:{}", utils::basename(__FILE__), __LINE__,
               inet_ntoa(addr.sin_addr), addr.sin_port);

  uint8_t buffer[1024]{};
  int nbytes = ::write(fd, "Hello World!", 12);
  if (nbytes < 0) {
    spdlog::error("{}:{} write(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    close(fd);
    return -1;
  }
  nbytes = ::read(fd, buffer, sizeof(buffer));
  if (nbytes < 0) {
    spdlog::error("{}:{} read(): {}", utils::basename(__FILE__), __LINE__,
                  strerror(errno));
    close(fd);
    return -1;
  }
  buffer[nbytes] = '\0';
  spdlog::info("{}:{} read(): {}", utils::basename(__FILE__), __LINE__,
               (char *)buffer);
  close(fd);
  return 0;
}
} // namespace
int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  spdlog::info("{}:{} client()", utils::basename(__FILE__), __LINE__);
  return client("127.0.0.1", 12345);
}
