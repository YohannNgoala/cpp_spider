#include "BoostNetwork.h"

BoostNetwork::BoostNetwork(boost::asio::io_service& io_service,
	boost::asio::ssl::context &context) :
	_port("4242"), _host("10.41.173.168"),
	_client(new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(_io_service, context)),
	_isConnected(false)
{
}

BoostNetwork::BoostNetwork(const std::string& port, const std::string& host,
	boost::asio::io_service& io_service,
	boost::asio::ssl::context &context) :
	_port(port), _host(host), _client(new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(_io_service, context))
{
}

int													BoostNetwork::send(const std::vector<char>& msg)
{
	_client->write_some(boost::asio::buffer(msg, msg.size()));
	return 0;
}

void												BoostNetwork::resetConnection()
{
	_io_service.reset();
	boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);
	context.load_verify_file("server.crt");
	_client.reset(new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(_io_service, context));
	tryConnection();
	std::this_thread::sleep_for(std::chrono::microseconds(1000));
}

int													BoostNetwork::receive()
{
	try
	{
		int												l = 1024;
		std::vector<char>								buf(l);
		boost::system::error_code						error;
		_client->read_some(boost::asio::buffer(buf));

		int code = static_cast<int>(buf[0]);;
		return (code);
	}
	catch (const std::exception &e)
	{
		resetConnection();
		return 0;
	}
}

void												BoostNetwork::handle_handshake(const boost::system::error_code& error)
{
	_isConnected = true;
	std::this_thread::sleep_for(std::chrono::microseconds(1000));
	if (error)
	{
		std::cout << "Handshake failed: " << error.message() << "\n";
		return resetConnection();
	}
}

void												BoostNetwork::handle_connect(const boost::system::error_code& error)
{
	if (!error)
	{
		_client->async_handshake(boost::asio::ssl::stream_base::client,
			boost::bind(&BoostNetwork::handle_handshake, this,
				boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Connect failed: " << error.message() << "\n";
		return resetConnection();
	}
}

void												BoostNetwork::tryConnection()
{
	try
	{
		std::cout << "connection ..." << std::endl;
		_client->set_verify_mode(boost::asio::ssl::verify_peer);
		boost::asio::ip::tcp::resolver					resolver(_io_service);
		boost::asio::ip::tcp::resolver::query			query(_host.c_str(), _port.c_str());
		boost::asio::ip::tcp::resolver::iterator		iterator = resolver.resolve(query);
		boost::asio::async_connect(_client->lowest_layer(), iterator,
			boost::bind(&BoostNetwork::handle_connect, this,
				boost::asio::placeholders::error));
		run();
	}
	catch (const std::exception &e)
	{
		return resetConnection();
	}
}

bool												BoostNetwork::disconnect()
{
	_io_service.stop();
	_client->lowest_layer().cancel();
	return (true);
}

void												BoostNetwork::run()
{
		_io_service.run();
}