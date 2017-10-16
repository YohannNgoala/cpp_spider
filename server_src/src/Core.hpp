//
// Core.hpp for spider in /home/matteo/rendu/cpp_spider/src
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Tue Oct  3 16:28:19 2017 matteo
// Last update Sun Oct  8 15:48:40 2017 matteo
//

#ifndef CORE_HPP_
#define CORE_HPP_

#include "BoostServer.hpp"
#include "CommandHandler.hpp"
#include "DataHandler.hpp"

class Core {
  bool _quit;
  std::mutex _clientMutex;
  std::unique_ptr<INetwork> _network;
  std::list<std::shared_ptr<IClient>> _clients;
  DataHandler _dataHandler;
  CommandHandler _commandHandler;
  std::thread _commandThread;

public:
  Core(void);
  ~Core(void);

  void start(unsigned short port);

private:
  Core(const Core &cp) = default;
  Core &operator=(const Core &cp) = default;
};

#endif // CORE_HPP_
