//
// Session.cpp for spider in /home/matteo/rendu/cpp_spider/server_src
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Thu Oct  5 12:34:47 2017 matteo
// Last update Sun Oct  8 16:21:15 2017 matteo
//

#include "Protocol.hpp"
#include "Session.hpp"
#include <boost/bind.hpp>

Session::Session(boost::asio::io_service &ioService, unsigned id,
                 boost::asio::ssl::context &context,
                 std::condition_variable &threadDeath)
    : _socket(ioService, context), _id(id), _isDead(false), _writeReady(false),
      _threadDeath(threadDeath) {}

Session::sessionPointer Session::create(boost::asio::io_service &ioService,
                                        boost::asio::ssl::context &context,
                                        std::condition_variable &threadDeath) {
  static unsigned id = 0;
  return (sessionPointer(new Session(ioService, id++, context, threadDeath)));
}

Session::ssl_socket::lowest_layer_type &Session::getSocket(void) {
  return (_socket.lowest_layer());
}

void Session::start(void) {
  _socket.async_handshake(boost::asio::ssl::stream_base::server,
                          boost::bind(&Session::handleHandshake, this,
                                      boost::asio::placeholders::error));
}

void Session::doRead(void) {
  _buffer.fill(0);
  boost::asio::async_read(_socket, boost::asio::buffer(_buffer, _buffer.size()),
                          boost::asio::transfer_at_least(1),
                          boost::bind(&Session::handleRead, shared_from_this(),
                                      boost::asio::placeholders::error));
}

void Session::handleRead(const boost::system::error_code &error) {
  if (!error) {
    std::vector<char> msg;
    for (auto &it : _buffer)
      msg.push_back(it);
    _dataQueue.emplace(msg);
    doRead();
  } else
    notifyDeath();
}

void Session::handleHandshake(const boost::system::error_code &error) {
  if (!error) {
    sendState(Spider::ASK_ID);
    _writeReady = true;
    doRead();
  } else if (_isDead == false)
    notifyDeath();
}

void Session::sendState(int state) {
  _stateBuf[0] = state;
  boost::asio::write(_socket, boost::asio::buffer(_stateBuf));
}

std::vector<char> Session::receiveData(void) {
  if (_dataQueue.empty())
    throw std::runtime_error("Try to pop an empty queue");
  std::vector<char> tmp(_dataQueue.front());
  _dataQueue.pop();
  return (tmp);
}

bool Session::writeReady(void) const { return (_writeReady); }

bool Session::dataAvailable(void) { return (!_dataQueue.empty()); }

bool Session::isDead(void) const { return (_isDead); }

unsigned Session::getId(void) const { return (_id); }

void Session::notifyDeath(void) {
  _writeReady = false;
  _isDead = true;
  _threadDeath.notify_all();
}
