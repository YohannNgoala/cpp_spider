#ifndef CONTROLDATA_HPP_
# define CONTROLDATA_HPP_

#include "WindowsInput.hpp"
#include "Status.hpp"
#include "SafeQueue.hpp"


struct										ControlData
{
	bool									_isConnected = false;
	SafeQueue<std::shared_ptr<IInput> >		_keys;
	Status									_state;
};

#endif // !CONTROLDATA_HPP_