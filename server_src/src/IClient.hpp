//
// IClient.hpp for spider in /home/matteo/rendu/spider
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Sun Oct  1 17:36:23 2017 matteo
// Last update Sun Oct  8 00:04:34 2017 matteo
//

#ifndef ICLIENT_HPP_
#define ICLIENT_HPP_

#include <vector>

class IClient {
public:
  virtual ~IClient(void) {}

  virtual void sendState(int state) = 0;
  virtual std::vector<char> receiveData(void) = 0;
  virtual unsigned getId(void) const = 0;
  virtual bool dataAvailable(void) = 0;
  virtual bool writeReady(void) const = 0;
};

#endif // ICLIENT_HPP_
