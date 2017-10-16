#ifndef IDATA_HPP_
#define IDATA_HPP_

#include <memory>
#include <vector>

class IData {
public:
  virtual ~IData(void) = default;

  virtual void dump(void) const = 0;
  virtual std::string getData(void) const = 0;
  virtual std::shared_ptr<IData> cloneObj(const std::vector<char> &data) = 0;
  virtual void feedObj(const std::vector<char> &data) = 0;
};

#endif /* IDATA_HPP_ */
