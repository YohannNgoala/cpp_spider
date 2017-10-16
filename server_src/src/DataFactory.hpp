#ifndef DATAFACTORY_HPP
#define DATAFACTORY_HPP

#include <map>
#include <memory>
#include <vector>

template <class DataObject, class Key = int> class DataFactory {
private:
  std::map<Key, std::shared_ptr<DataObject>> _instanceCreator;

public:
  DataFactory(void) = default;
  ~DataFactory(void) = default;

  void feedCreator(Key key, std::shared_ptr<DataObject> dataObj) {
    if (_instanceCreator.find(key) == _instanceCreator.end())
      _instanceCreator[key] = dataObj;
  }

  std::shared_ptr<DataObject> createInstance(const Key &key,
                                             const std::vector<char> &data) {
    std::shared_ptr<DataObject> tmpObj = NULL;
    auto it = _instanceCreator.find(key);

    if (it != _instanceCreator.end())
      tmpObj = ((*it).second)->cloneObj(data);
    return (tmpObj);
  }

private:
  DataFactory(const DataFactory &cp) = delete;
  DataFactory &operator=(const DataFactory &cp) = delete;
};

#endif /* DATAFACTORY_HPP */
