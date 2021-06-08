#pragma once
#include <boost/asio.hpp>
#include "utils.h"

//static class for all the server sock handling 

class server_sock
{
private:
	static void read_from_server(boost::asio::ip::tcp::socket& socket, unsigned char* buffer_pointer, int amount_of_bytes_to_read);
	static void append_to_buffer_from_sock(boost::asio::ip::tcp::socket& socket, unsigned char* buffer_pointer, int amount_of_bytes_to_add, int add_starting_from);
	static void connect(boost::asio::ip::tcp::socket& socket);
	static void write_to_server(boost::asio::ip::tcp::socket& socket, unsigned char* buffer_pointer, int amount_of_bytes_to_send);
	static void read_response_from_server(boost::asio::ip::tcp::socket& socket, unsigned char* buffer_pointer);
	static void close_connection(boost::asio::ip::tcp::socket& socket);
public:
	static void connect_send_and_get(boost::asio::ip::tcp::socket& socket, unsigned char* write_buffer_pointer, int amount_of_bytes_to_send);
};
