#ifndef MOUSEDATA_HPP_
#define MOUSEDATA_HPP_

#include "IData.hpp"
#include "Protocol.hpp"
#include <unordered_map>

class MouseData : public IData {
public:
  MouseData(void);
  MouseData(const std::vector<char> &data);

  virtual ~MouseData(void) = default;

private:
  int _timestamp;
  char _procName[256];
  std::pair<int, int> _pos;
  Spider::ClickType _clickType;

public:
  virtual void dump(void) const;
  virtual std::string getData(void) const;
  virtual std::shared_ptr<IData> cloneObj(const std::vector<char> &data);
  virtual void feedObj(const std::vector<char> &data);

private:
  MouseData(const MouseData &cp) = delete;
  MouseData &operator=(const MouseData &cp) = delete;
};

#endif /* MOUSEDATA_HPP_ */
