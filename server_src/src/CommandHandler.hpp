//
// CommandHandler.hpp for spider in /home/matteo/tmp/cpp_spider/server_src/src
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Sat Oct  7 21:11:23 2017 matteo
// Last update Sun Oct  8 15:51:38 2017 matteo
//

#ifndef COMMANDHANDLER_HPP_
#define COMMANDHANDLER_HPP_

#include "IClient.hpp"
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>

class CommandHandler {
  typedef void (CommandHandler::*Command)(
      std::vector<std::string> const &command);

  std::unordered_map<std::string, Command> _commands;
  std::list<std::shared_ptr<IClient>> &_clients;
  std::mutex &_clientMutex;
  bool &_coreQuit;

public:
  CommandHandler(std::list<std::shared_ptr<IClient>> &clients,
                 std::mutex &clientMutex, bool &coreQuit);
  ~CommandHandler(void) = default;

  void executeCommand(std::string const &line);

private:
  std::vector<std::string> parseCommand(std::string const &command);
  void help(std::vector<std::string> const & /* command */);
  void send(std::vector<std::string> const &command);
  void list(std::vector<std::string> const & /* command */);
  void requestCodes(std::vector<std::string> const & /* command */);
  void quit(std::vector<std::string> const & /* command */);
  void prompt(void);
};

#endif // COMMANDHANDLER_HPP_
