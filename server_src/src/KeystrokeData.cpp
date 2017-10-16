#include "KeystrokeData.hpp"
#include "Protocol.hpp"
#include "TimestampHandle.hpp"
#include "VirtualKeysMap.hpp"
#include <cstring>
#include <iostream>

KeystrokeData::KeystrokeData(void) : _timestamp(0), _key(0) {}

KeystrokeData::KeystrokeData(const std::vector<char> &data)

{
  if (data.size() < sizeof(int) + sizeof(Spider::t_keyboard_event)) {
    std::cerr << "Invalid size of packet for VALID_KEY_EVENT." << std::endl;
    return;
  }
  _timestamp = *(reinterpret_cast<const int *>(&data[sizeof(int)]));
  _key = *(reinterpret_cast<const int *>(
      &data[sizeof(int) + sizeof(_timestamp) + sizeof(char[256])]));

  std::memcpy(&_procName[0], &data[sizeof(int) + sizeof(_timestamp)],
              sizeof(_procName));
}

std::shared_ptr<IData> KeystrokeData::cloneObj(const std::vector<char> &data) {
  return (std::make_shared<KeystrokeData>(data));
}

void KeystrokeData::dump(void) const {
  /*
  std::cout << TimestampHandle::timeStampToString(_timestamp)
            << std::string(" -- type=Keystroke : processusName=") << _procName
            << " -- Key=" << virtualKeysMap[_key] << std::endl;
  */
}

std::string KeystrokeData::getData(void) const {
  return ("type=Keystroke time=[" +
          TimestampHandle::timeStampToString(_timestamp) +
          +"] ProcName=" + _procName + " input=" + virtualKeysMap[_key]);
}

void KeystrokeData::feedObj(const std::vector<char> &data) {
  if (data.size() < sizeof(int) + sizeof(Spider::t_keyboard_event)) {
    std::cerr << "Invalid size of packet for VALID_KEY_EVENT." << std::endl;
    return;
  }
  _timestamp = *(reinterpret_cast<const int *>(&data[sizeof(int)]));
  memcpy(&_procName[0], &data[sizeof(int) + sizeof(_timestamp)],
         sizeof(_procName));
  _key = *(reinterpret_cast<const int *>(
      &data[sizeof(int) + sizeof(_timestamp) + sizeof(_procName)]));
}
