#ifndef TCPSERVER_HPP_
#define TCPSERVER_HPP_
#include "Business.hpp"
#include <string>
#include <uv.h>

class TCPServer {
public:
  TCPServer(std::string hostIp, int port, Business *business);
  ~TCPServer();
  void Run();
  // void Stop();
  void Init();
  void Bind();
  void Listen();

protected:
  static void OnNewConnection(uv_stream_t *server, int status);
  static void AllocBuffer(uv_handle_t *handle, size_t suggested_size,
                          uv_buf_t *buf);
  static void OnRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
  static void AfterWrite(uv_write_t *req, int status);

protected:
  struct write_req_t {
    uv_write_t req;
    uv_buf_t buf;
  };

  struct tcp_business_t {
    uv_tcp_t server;
    Business *business;
  };

protected:
  std::string _hostIp;
  int _port;
  tcp_business_t _tb;
};

#endif