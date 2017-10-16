//
// server.hpp for server in /home/matteo/rendu/misc/async_server_boost
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Thu Sep 28 14:23:10 2017 matteo
// Last update Sat Oct  7 15:11:15 2017 matteo
//

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "Session.hpp"
#include <atomic>
#include <list>
#include <mutex>
#include <thread>

extern std::mutex clientMutex;

class Server {
  std::condition_variable _threadDeath;
  std::mutex &_clientMutex;
  tcp::acceptor _acceptor;
  std::atomic<bool> _end;
  std::thread _clientCheck;
  std::list<std::shared_ptr<IClient>> &_clients;
  boost::asio::ssl::context _context;

public:
  Server(boost::asio::io_service &ioService, int port,
         std::list<std::shared_ptr<IClient>> &clients, std::mutex &clientMutex);
  ~Server(void);

private:
  void startAccept(void);
  void handleAccept(Session::sessionPointer &newClient,
                    const boost::system::error_code &error);

private:
  Server(const Server &cp) = delete;
  Server &operator=(const Server &cp) = delete;
};

#endif // SERVER_HPP_
