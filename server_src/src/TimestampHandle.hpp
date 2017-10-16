#ifndef TIMESTAMPHANDLE_HPP_
#define TIMESTAMPHANDLE_HPP_

#include <ctime>
#include <locale>
#include <string>

class TimestampHandle {
private:
  TimestampHandle(void) = delete;
  TimestampHandle(const TimestampHandle &cp) = delete;
  TimestampHandle &operator=(const TimestampHandle &cp) = delete;
  ~TimestampHandle(void) = default;

public:
  static std::string timeStampToString(const int &timeStamp) {
    time_t t = timeStamp;
    char tmp[100];
    std::string res;

    if (std::strftime(tmp, sizeof(tmp), "%c", std::localtime(&t)))
      res = tmp;

    return (res);
  }
  static int stringToTimestamp(const std::string &ct) {
    struct tm tm;
    strptime(ct.c_str(), "%c", &tm);
    return timegm(&tm);
  }
};

#endif /* TIMESTAMPHANDLE_HPP_ */
