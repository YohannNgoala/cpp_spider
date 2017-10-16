#include "StatusData.hpp"
#include "TimestampHandle.hpp"
#include <cstring>
#include <iostream>

StatusData::StatusData(void) : _status({false, false, false, false}) {}

StatusData::StatusData(const std::vector<char> &data) {
  if (data.size() < sizeof(int) + sizeof(Spider::t_status)) {
    std::cerr << "Invalid size of packet for VALID_GET_STATUS_RESPONSE."
              << std::endl;
    return;
  }
  _status = *(reinterpret_cast<const Spider::t_status *>(&data[sizeof(int)]));
}

std::shared_ptr<IData> StatusData::cloneObj(const std::vector<char> &data) {
  return (std::make_shared<StatusData>(data));
}

void StatusData::dump(void) const {
  std::cout << std::boolalpha << "type=Status :" << std::endl
            << "Tracking Status : \n\t"
            << "keyboard=" << _status.keyboard_tracking << "\n\t"
            << "Mouse click=" << _status.mouse_click_tracking << "\n\t"
            << "Mouse Move=" << _status.mouse_move_tracking << "\n\t"
            << "Pause=" << _status.paused << std::noboolalpha << std::endl;
  std::cout << "server >> " << std::flush;
}

std::string StatusData::getData(void) const {
  return ("type=Status Keyboard=" + boolToString(_status.keyboard_tracking) +
          " MouseClick=" + boolToString(_status.mouse_click_tracking) +
          " MouseMove=" + boolToString(_status.mouse_move_tracking) +
          " Pause=" + boolToString(_status.paused));
}

void StatusData::feedObj(const std::vector<char> &data) {
  if (data.size() < sizeof(int) + sizeof(Spider::t_status)) {
    std::cerr << "Invalid size of packet for VALID_GET_STATUS_RESPONSE."
              << std::endl;
    return;
  }
  _status = *(reinterpret_cast<const Spider::t_status *>(&data[sizeof(int)]));
}

const std::string StatusData::boolToString(const bool &val) const {
  return (val ? "True" : "False");
}
