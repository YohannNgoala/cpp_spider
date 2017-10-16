#ifndef DATAHANDLER_HPP_
#define DATAHANDLER_HPP_

#include "DataFactory.hpp"
#include "IData.hpp"
#include "Protocol.hpp"
#include <functional>
#include <unordered_map>
#include <utility>

class DataHandler {
public:
  DataHandler(void);
  ~DataHandler(void) = default;

  void handleData(const int &id, const std::vector<char> &data);

private:
  DataFactory<IData> _factory;
  std::map<Spider::RequestCode, std::string> _mapRequestsMessages;
  std::map<int, std::pair<std::string, std::string>> _mapClients;
  std::unordered_map<
      Spider::RequestCode,
      std::function<void(DataHandler &, const int &,
                         const std::pair<std::string, std::string> &hostUser,
                         const std::vector<char> &data)>>
      _requestHandler;

private:
  DataHandler(const DataHandler &cp) = delete;
  DataHandler &operator=(const DataHandler &cp) = delete;

  void feedFactory(void);
  void feedDataMaps(void);
  void treatId(const int &id);
  bool isIdSet(const int &id);

  // data treatment

  void handleIdResponse(const int &id,
                        const std::pair<std::string, std::string> &hostUser,
                        const std::vector<char> &data);
  void dumpData(const int &id,
                const std::pair<std::string, std::string> &hostUser,
                const std::vector<char> &data);

  void PrintCodeResponse(const int &id,
                         const std::pair<std::string, std::string> &hostUser,
                         const std::vector<char> &data);
};

#endif /* DATAHANDLER_HPP_ */
