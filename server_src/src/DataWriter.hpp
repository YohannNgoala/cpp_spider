#ifndef DATAWRITER_HPP_
#define DATAWRITER_HPP_

#include <fstream>
#include <string>

#include <unordered_map>

const std::string _dataDir = "dataDirectory";

struct t_timeSmartLogs {
  int _startSequenceTime;
  int _currentSequenceTime;
  std::string _sequence;
};

class DataWriter {
public:
  DataWriter(void) = default;
  ~DataWriter(void) = default;

  void writeData(const std::string &host, const std::string &user,
                 const std::string &data);

private:
  std::unordered_map<std::string, std::ofstream> _fileStreamsBasic;
  std::unordered_map<std::string, std::ofstream> _fileStreamsTimeSmart;

  std::unordered_map<std::string, t_timeSmartLogs> _sequenceData;

  void tryInitDirectory(const std::string &host) const;

  void writeBasicData(const std::string &host, const std::string &user,
                      const std::string &data);
  void writeTimeBasedSmartData(const std::string &host, const std::string &user,
                               const std::string &data);

  void resetSequenceData(const std::string &host, const std::string &user);

  void feedSequenceWriter(const std::string &host, const std::string &user,
                          const std::string &data);

private:
  DataWriter(const DataWriter &cp) = default;
  DataWriter &operator=(const DataWriter &cp) = delete;
};

#endif /* DATAWRITER_HPP_ */
