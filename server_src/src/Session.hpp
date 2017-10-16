//
// Session.hpp for spider in /home/matteo/rendu/spider/tmp
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Sun Oct  1 18:12:45 2017 matteo
// Last update Sun Oct  8 16:20:38 2017 matteo
//

#ifndef SESSION_HPP_
#define SESSION_HPP_

#include "IClient.hpp"
#include <array>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>, public IClient {
  typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

  ssl_socket _socket;
  unsigned _id;
  std::array<char, 1024> _buffer;
  std::array<int, 1> _stateBuf;
  bool _isDead;
  bool _writeReady;
  std::condition_variable &_threadDeath;
  std::queue<std::vector<char>> _dataQueue;

public:
  ~Session(void) = default;

  typedef std::shared_ptr<Session> sessionPointer;

  static sessionPointer create(boost::asio::io_service &ioService,
                               boost::asio::ssl::context &context,
                               std::condition_variable &threadDeath);

  ssl_socket::lowest_layer_type &getSocket(void);
  void start(void);
  bool isDead(void) const;
  void notifyDeath(void);
  void doRead(void);

  void sendState(int state) override final;
  std::vector<char> receiveData(void) override final;
  unsigned getId(void) const override final;
  bool dataAvailable(void) override final;
  bool writeReady(void) const override final;

private:
  Session(boost::asio::io_service &ioService, unsigned id,
          boost::asio::ssl::context &context,
          std::condition_variable &threadDeath);
  Session(const Session &cp) = delete;
  Session &operator=(const Session &cp) = delete;

  void handleRead(const boost::system::error_code &error);
  void handleHandshake(const boost::system::error_code &error);
};

#endif // SESSION_HPP_
