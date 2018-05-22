#ifndef EVENTLOOP_HPP_
#define EVENTLOOP_HPP_
#include "TCPServer.hpp"
#include <list>
#include <uv.h>

class EventLoop {
public:
  static uv_loop_t *loop;
  void Run();
  void AddServer(TCPServer *server);

private:
  std::list<TCPServer *> _servers;
};

#endif