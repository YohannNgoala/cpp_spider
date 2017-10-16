#include "MouseData.hpp"
#include "Protocol.hpp"
#include "TimestampHandle.hpp"
#include <cstring>
#include <iostream>

MouseData::MouseData(void)
    : _timestamp(0), _pos({0, 0}), _clickType(Spider::NONE) {}

MouseData::MouseData(const std::vector<char> &data) {
  if (data.size() < sizeof(int) + sizeof(Spider::VALID_MOUSE_EVENT)) {
    std::cerr << "Invalid size of packet for VALID_MOUSE_EVENT." << std::endl;
    return;
  }
  _timestamp = *(reinterpret_cast<const int *>(&data[sizeof(int)]));
  _pos = {
      *(reinterpret_cast<const int *>(
          &data[sizeof(int) + sizeof(_timestamp) + sizeof(_procName)])),
      *(reinterpret_cast<const int *>(&data[sizeof(int) + sizeof(_timestamp) +
                                            sizeof(_procName) + sizeof(int)]))};
  _clickType = *(reinterpret_cast<const Spider::ClickType *>(
      &data[sizeof(int) + sizeof(_timestamp) + sizeof(_procName) + sizeof(int) +
            sizeof(int)]));

  std::memcpy(&_procName[0], &data[sizeof(int) + sizeof(_timestamp)],
              sizeof(_procName));
}

std::shared_ptr<IData> MouseData::cloneObj(const std::vector<char> &data) {
  return (std::make_shared<MouseData>(data));
}

void MouseData::dump(void) const {
  /*
    static std::unordered_map<Spider::ClickType, std::string> _clickName = {
        {Spider::NONE, "NONE"},
        {Spider::LEFT, "Left click"},
        {Spider::RIGHT, "Right click"},
        {Spider::OTHER, "Other button click"}};

    std::string tmpClickType = _clickType >= Spider::OTHER
                                   ? _clickName[Spider::OTHER]
                                   : _clickName[_clickType];
    std::cout << TimestampHandle::timeStampToString(_timestamp)
              << std::string(" -- type=Mouse : processusName=") << _procName
              << " -- Input=" << tmpClickType << " -- Position=x:" << _pos.first
              << "y:" << _pos.second << std::endl;
  */
}

std::string MouseData::getData(void) const {
  static std::unordered_map<Spider::ClickType, std::string> _clickName = {
      {Spider::NONE, "NONE"},
      {Spider::LEFT, "Left click"},
      {Spider::RIGHT, "Right click"},
      {Spider::OTHER, "Other button click"}};

  std::string tmpClickType = _clickType >= Spider::OTHER
                                 ? _clickName[Spider::OTHER]
                                 : _clickName[_clickType];

  return ("type=Mouse time=[" + TimestampHandle::timeStampToString(_timestamp) +
          +"] procName=" + _procName + " input=" + tmpClickType +
          " positionx=" + std::to_string(_pos.first) +
          " positiony=" + std::to_string(_pos.second));
}

void MouseData::feedObj(const std::vector<char> &data) {
  if (data.size() < sizeof(int) + sizeof(Spider::VALID_MOUSE_EVENT)) {
    std::cerr << "Invalid size of packet for VALID_MOUSE_EVENT." << std::endl;
    return;
  }
  _timestamp = *(reinterpret_cast<const int *>(&data[sizeof(int)]));
  _pos = {
      *(reinterpret_cast<const int *>(
          &data[sizeof(int) + sizeof(_timestamp) + sizeof(_procName)])),
      *(reinterpret_cast<const int *>(&data[sizeof(int) + sizeof(_timestamp) +
                                            sizeof(_procName) + sizeof(int)]))};
  _clickType = *(reinterpret_cast<const Spider::ClickType *>(
      &data[sizeof(int) + sizeof(_timestamp) + sizeof(_procName) + sizeof(int) +
            sizeof(int)]));
  std::memcpy(&_procName[0], &data[sizeof(int) + sizeof(_timestamp)],
              sizeof(_procName));
}
