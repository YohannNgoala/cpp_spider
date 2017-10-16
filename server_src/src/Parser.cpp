#include "Parser.hpp"
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_map>

Parser::Parser(const std::string &data) : _type(getKeyData("type=", data)) {
  std::unordered_map<std::string,
                     std::function<void(Parser &, const std::string &data)>>
      _mapFeeder = {{"Mouse", &Parser::feedMouseData},
                    {"Status", &Parser::feedStatusData},
                    {"Keystroke", &Parser::feedKeystrokeData}};
  if (_mapFeeder.find(_type) == _mapFeeder.end())
    return;
  _mapFeeder[_type](*this, data);
}

std::string Parser::getKeyData(const std::string &key,
                               const std::string &data) {
  int startPos = data.find(key) + key.size();
  int endPos = data.find(key == "time=[" ? "]" : " ", startPos);
  return (data.substr(startPos, endPos - startPos));
}

void Parser::feedMouseData(const std::string &data) {
  _time = getKeyData("time=[", data);
  _procName = getKeyData("procName=", data);
  _input = getKeyData("input=", data);
  std::istringstream(getKeyData("positionx=", data)) >> _posx;
  std::istringstream(getKeyData("positiony=", data)) >> _posy;
}

void Parser::feedStatusData(const std::string &data) {
  _keyboardTrack = stringToBool(getKeyData("Keyboard=", data));
  _mouseClickTrack = stringToBool(getKeyData("MouseClick=", data));
  _mouseMoveTrack = stringToBool(getKeyData("MouseMove=", data));
  _pause = stringToBool(getKeyData("Pause=", data));
}

void Parser::feedKeystrokeData(const std::string &data) {
  _time = getKeyData("time=[", data);
  _procName = getKeyData("ProcName=", data);
  _input = getKeyData("input=", data);
}

bool Parser::stringToBool(const std::string &data) {
  return (data == "True" ? true : false);
}
