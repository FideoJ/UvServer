#ifndef BUSINESS_HPP_
#define BUSINESS_HPP_
#include "Buffer.hpp"

class Business {
public:
  virtual void OnRead(const Buffer &recvBuf, Buffer &sendBuf) const = 0;
};

#endif