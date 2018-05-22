#include "Buffer.hpp"
#include "Business.hpp"
#include "EventLoop.hpp"
#include "TCPServer.hpp"
#include <cstring>
#include <string>

class EchoBusiness : public Business {
  void OnRead(const Buffer &recvBuf, Buffer &sendBuf) const {
    size_t nread = recvBuf.len;
    strncpy(sendBuf.base, recvBuf.base, nread);
    sendBuf.base[nread] = '\0';
    sendBuf.len = nread;
  }
};

int main() {
  std::string hostIp = "0.0.0.0";
  EchoBusiness echo;
  TCPServer server(hostIp, 7000, &echo);
  EventLoop el;

  el.AddServer(&server);
  el.Run();

  return 0;
}