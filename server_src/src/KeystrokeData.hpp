#ifndef KEYSTROKEDATA_HPP_
#define KEYSTROKEDATA_HPP_

#include "IData.hpp"

class KeystrokeData : public IData {
public:
  KeystrokeData(void);
  KeystrokeData(const std::vector<char> &data);

  virtual ~KeystrokeData(void) = default;

protected:
  int _timestamp;
  int _key;
  char _procName[256];

public:
  virtual void dump(void) const;
  virtual std::string getData(void) const;
  virtual std::shared_ptr<IData> cloneObj(const std::vector<char> &data);
  virtual void feedObj(const std::vector<char> &data);

private:
  KeystrokeData(const KeystrokeData &cp) = delete;
  KeystrokeData &operator=(const KeystrokeData &cp) = delete;
};

#endif /* KEYSTROKEDATA_HPP_ */
