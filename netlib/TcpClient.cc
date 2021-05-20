#include "./TcpClient.h"
#include "./Logging.h"
#include "./Connector.h"

using namespace netlib;

TcpClient::TcpClient(EventLoop *loop,
                     const SockAddr &addr,
                     const std::string &name)
    : _loop(loop),
      _name(name),
      _connector(new Connector(loop, addr)),
      _connect(false)
{
}

TcpClient::~TcpClient() {
    //
}

void TcpClient::connect() {
    /// 发起连接
    LOG_INFO << "TcpClient::connect [" << _name << "] -connecting to "
             << _connector->serverAddress().toIpPort();

    _connect = true;
    _connector->start();
}