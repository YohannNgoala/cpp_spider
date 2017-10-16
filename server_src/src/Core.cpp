//
// Core.cpp for spider in /home/matteo/rendu/cpp_spider/src
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Tue Oct  3 16:28:15 2017 matteo
// Last update Sun Oct  8 15:55:33 2017 matteo
//

#include "Core.hpp"

Core::Core(void)
    : _quit(false), _network(new BoostServer),
      _commandHandler(_clients, _clientMutex, _quit), _commandThread([this]() {
        std::string line;
        while (_quit == false && std::getline(std::cin, line))
          _commandHandler.executeCommand(line);
        _clientMutex.lock();
        _quit = true;
        _clientMutex.unlock();
      }) {}

Core::~Core(void) {
  _clientMutex.lock();
  _clients.clear();
  _clientMutex.unlock();
  _commandThread.join();
}

void Core::start(unsigned short port) {
  _network->startServer(port, _clients, _clientMutex);
  while (_quit == false) {
    std::this_thread::sleep_for(std::chrono::microseconds(1));
    _clientMutex.lock();
    for (auto &it : _clients) {
      if (it->dataAvailable()) {
        std::vector<char> msg = it->receiveData();
        if (msg.size() < 4) {
          std::cerr << "Invalid Packet size (less than 4 bytes)." << std::endl;
          continue;
        }
        _dataHandler.handleData(it->getId(), msg);
      }
    }
    _clientMutex.unlock();
  }
}
