#include "TCPServer.hpp"
#include "EventLoop.hpp"
#include <cstdlib>
#include <string>
#include <uv.h>
#define DEFAULT_BACKLOG 128
#define SENDBUF_SIZE 1024

TCPServer::TCPServer(std::string hostIp, int port, Business *business)
    : _hostIp(hostIp), _port(port) {
  _tb.business = business;
}

TCPServer::~TCPServer() {}

void TCPServer::Init() { uv_tcp_init(EventLoop::loop, (uv_tcp_t *)&_tb); }

void TCPServer::Bind() {
  struct sockaddr_in addr;
  uv_ip4_addr(_hostIp.c_str(), _port, &addr);

  uv_tcp_bind((uv_tcp_t *)&_tb, (const struct sockaddr *)&addr, 0);
}

void TCPServer::Listen() {
  int r = uv_listen((uv_stream_t *)&_tb, DEFAULT_BACKLOG, OnNewConnection);
  if (r) {
    fprintf(stderr, "Listen error %s\n", uv_strerror(r));
    exit(1);
  }
}

void TCPServer::OnNewConnection(uv_stream_t *server, int status) {
  if (status < 0) {
    fprintf(stderr, "New connection error %s\n", uv_strerror(status));
    return;
  }

  uv_tcp_t *client = (uv_tcp_t *)malloc(sizeof(tcp_business_t));
  uv_tcp_init(EventLoop::loop, client);
  ((tcp_business_t *)client)->business = ((tcp_business_t *)server)->business;

  if (uv_accept(server, (uv_stream_t *)client) == 0) {
    uv_read_start((uv_stream_t *)client, AllocBuffer, OnRead);
  } else {
    uv_close((uv_handle_t *)client, NULL);
    free((tcp_business_t *)client);
  }
}

void TCPServer::AllocBuffer(uv_handle_t *handle, size_t suggested_size,
                            uv_buf_t *buf) {
  *buf = uv_buf_init((char *)malloc(suggested_size), suggested_size);
}

void TCPServer::OnRead(uv_stream_t *client, ssize_t nread,
                       const uv_buf_t *buf) {
  if (nread > 0) {
    Buffer recvBuf(buf->base, nread);
    Buffer sendBuf((char *)malloc(sizeof(char) * SENDBUF_SIZE), SENDBUF_SIZE);

    ((tcp_business_t *)client)->business->OnRead(recvBuf, sendBuf);

    write_req_t *req = (write_req_t *)malloc(sizeof(write_req_t));
    req->buf = uv_buf_init(sendBuf.base, sendBuf.len);

    uv_write((uv_write_t *)req, client, &req->buf, 1, AfterWrite);
    return;
  }

  if (nread < 0) {
    if (nread != UV_EOF)
      fprintf(stderr, "Read error %s\n", uv_err_name(nread));
    uv_close((uv_handle_t *)client, NULL);
    free((tcp_business_t *)client);
  }

  free(buf->base);
}

void TCPServer::AfterWrite(uv_write_t *req, int status) {
  if (status) {
    fprintf(stderr, "Write error %s\n", uv_strerror(status));
  }

  write_req_t *wr = (write_req_t *)req;
  free(wr->buf.base);
  free(wr);
}