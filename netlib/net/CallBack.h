#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "netlib/base/Time.h"

#include <functional>
#include <memory>

namespace netlib 
{

class TcpConnection;
class Buffer;


typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void(const TcpConnectionPtr&, Buffer*, Time)> MessageCallBack;
typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallBack;
typedef std::function<void(const TcpConnectionPtr&)> CloseCallBack;
typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallBack;
typedef std::function<void(const TcpConnectionPtr&, size_t)> HightWaterMarkCallBack;


void defaultConnectionCallBack(const TcpConnectionPtr &con);

void defaultMessageCallBack(const TcpConnectionPtr &con, Buffer *buf, Time time);

}


#endif