//
// main.cpp for spider in /home/matteo/rendu/spider
//
// Made by matteo
// Login   <matteo.margueritte@epitech.eu>
//
// Started on  Mon Oct  2 19:06:59 2017 matteo
// Last update Sun Oct  8 14:43:49 2017 matteo
//

#include "Core.hpp"
#include <iostream>

int main(int ac, char **av) {
  if (ac != 2) {
    std::cerr << "Usage : ./server [port]" << std::endl;
    return (EXIT_FAILURE);
  }
  try {
    Core core;
    std::cout << "server >> " << std::flush;
    core.start(std::stoi(av[1]));
  } catch (std::exception const &error) {
    std::cerr << error.what() << std::endl;
    return (EXIT_FAILURE);
  }
  return (EXIT_SUCCESS);
}
