# UvServer
## Introduction
- Base on libuv
- Use strategy pattern
- A toy for fun
## Demo
```
class HelloBusiness : public Business {
  void OnRead(const Buffer &recvBuf, Buffer &sendBuf) const {
    strncpy(sendBuf.base, "Hello world\n", 12);
    sendBuf.base[12] = '\0';
    sendBuf.len = 12;
  }
};

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
  HelloBusiness hello;
  EchoBusiness echo;
  TCPServer server1(hostIp, 7000, &hello), server2(hostIp, 7001, &echo);
  EventLoop el;

  el.AddServer(&server1);
  el.AddServer(&server2);
  el.Run();

  return 0;
}
```