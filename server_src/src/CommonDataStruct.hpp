#ifndef COMMONDATASTRUCT_HPP_
#define COMMONDATASTRUCT_HPP_

#include "EnumRequestsTypes.hpp"
#include <chrono>
#include <string>

/* Position for mouse tracking */
typedef std::pair<int, int> Position;

struct HeaderData {
  RequestCode _requestCode;
  std::chrono::time_point<std::chrono::system_clock> _time;
};

struct UserData {
  std::string _userId;
  std::string _processName;
};

#endif /* COMMONDATASTRUCT_HPP_ */
