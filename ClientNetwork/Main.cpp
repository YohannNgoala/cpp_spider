#include "BoostNetwork.h"
#include "Client.hpp"
#include "stdafx.h"

int main(int argc, char **argv)
{
	try
	{
		ShowWindow(GetConsoleWindow(), 0);
		boost::asio::io_service io_service;
		boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);

		context.load_verify_file("server.crt");

		Client					c("4242", "10.41.173.168", io_service, context);

		c.connectClient();
	}
	catch (std::exception &e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}