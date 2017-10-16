//
// BoostServer.hpp for spider in /home/matteo/rendu/spider
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Sat Sep 30 17:40:13 2017 matteo
// Last update Sat Oct  7 15:03:41 2017 matteo
//

#ifndef BOOSTSERVER_HPP_
#define BOOSTSERVER_HPP_

#include "INetwork.hpp"
#include "Server.hpp"
#include <boost/asio.hpp>

class BoostServer : public INetwork {
  boost::asio::io_service _ioService;
  std::unique_ptr<Server> _server;
  std::thread _running;

public:
  BoostServer(void) = default;
  virtual ~BoostServer(void);

  void startServer(unsigned short port,
                   std::list<std::shared_ptr<IClient>> &clients,
                   std::mutex &clientMutex) final;

private:
  BoostServer(const BoostServer &cp) = delete;
  BoostServer &operator=(const BoostServer &cp) = delete;
};

#endif // BOOSTSERVER_HPP_
