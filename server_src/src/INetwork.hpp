//
// INetwork.hpp for spider in /home/matteo/rendu/spider
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Sat Sep 30 17:31:27 2017 matteo
// Last update Sat Oct  7 15:03:11 2017 matteo
//

#ifndef INETWORK_HPP_
#define INETWORK_HPP_

#include "IClient.hpp"
#include <list>
#include <memory>
#include <mutex>
#include <string>

class INetwork {
public:
  virtual ~INetwork(void) = default;

  virtual void startServer(unsigned short port,
                           std::list<std::shared_ptr<IClient>> &clients,
                           std::mutex &clientMutex) = 0;
};

#endif // INETWORK_HPP_
