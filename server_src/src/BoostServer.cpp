//
// BoostServer.cpp for spider in /home/matteo/rendu/spider
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Sat Sep 30 17:40:15 2017 matteo
// Last update Sat Oct  7 15:52:42 2017 matteo
//

#include "BoostServer.hpp"
#include <boost/asio/ip/address.hpp>

BoostServer::~BoostServer() {
  _ioService.stop();
  _running.join();
}

void BoostServer::startServer(unsigned short port,
                              std::list<std::shared_ptr<IClient>> &clients,
                              std::mutex &clientMutex) {
  _server = std::make_unique<Server>(_ioService, port, clients, clientMutex);
  _running = std::thread([this](void) { _ioService.run(); });
}
