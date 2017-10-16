#ifndef STATUSDATA_HPP_
#define STATUSDATA_HPP_

#include "IData.hpp"
#include "Protocol.hpp"

class StatusData : public IData {
public:
  StatusData(void);
  StatusData(const std::vector<char> &data);

  virtual ~StatusData(void) = default;

protected:
  Spider::t_status _status;

public:
  virtual void dump(void) const;
  virtual std::string getData(void) const;
  virtual std::shared_ptr<IData> cloneObj(const std::vector<char> &data);
  virtual void feedObj(const std::vector<char> &data);

private:
  StatusData(const StatusData &cp) = delete;
  StatusData &operator=(const StatusData &cp) = delete;

  const std::string boolToString(const bool &val) const;
};

#endif /* STATUSDATA_HPP_ */
