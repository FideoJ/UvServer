#include "EventLoop.hpp"
#include "TCPServer.hpp"
#include <uv.h>

uv_loop_t *EventLoop::loop = uv_default_loop();

void EventLoop::Run() { uv_run(EventLoop::loop, UV_RUN_DEFAULT); }

void EventLoop::AddServer(TCPServer *server) {
  _servers.push_back(server);
  server->Init();
  server->Bind();
  server->Listen();
}