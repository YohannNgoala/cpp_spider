#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <string>

class Parser {
public:
  Parser(void) = default;
  Parser(const std::string &data);
  ~Parser(void) = default;

public:
  std::string _type;

  std::string _time;
  std::string _procName;
  std::string _input;

  int _posx;
  int _posy;

  bool _keyboardTrack;
  bool _mouseClickTrack;
  bool _mouseMoveTrack;
  bool _pause;

private:
  Parser(const Parser &cp) = delete;
  Parser &operator=(const Parser &cp) = delete;

  std::string getKeyData(const std::string &key, const std::string &data);

  // Feeders
  void feedMouseData(const std::string &data);
  void feedStatusData(const std::string &data);
  void feedKeystrokeData(const std::string &data);

  // tools
  bool stringToBool(const std::string &data);
};

#endif /* PARSER_HPP_ */
