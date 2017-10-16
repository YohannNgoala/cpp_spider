#ifndef INETWORK_H_
#define INETWORK_H_

#include <vector>

class				INetwork
{
public:
	virtual ~INetwork() = default;
	virtual int		send(const std::vector<char>& msg) = 0;
	virtual int		receive() = 0;
	virtual void	tryConnection() = 0;
	virtual bool	disconnect() = 0;
	virtual bool	isConnected() = 0;
	virtual void	setIsConnected(bool connected) = 0;
	virtual void	run() = 0;
};

#endif // !INETWORK_H_