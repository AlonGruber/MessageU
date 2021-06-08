#include "server_sock.h"
#include "Responses.h"

void server_sock::connect(boost::asio::ip::tcp::socket& socket)
{
	socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(utils::get_server_ip()), utils::get_server_port()));
}

void server_sock::write_to_server(boost::asio::ip::tcp::socket &socket,unsigned char * buffer_pointer,int amount_of_bytes_to_send)
{
	boost::asio::write(socket, boost::asio::buffer(buffer_pointer, amount_of_bytes_to_send));
}

void server_sock::read_response_from_server(boost::asio::ip::tcp::socket& socket, unsigned char* buffer_pointer)
{
	read_from_server(socket, buffer_pointer, RESPONSE_HEADER_BYTE_SIZE);
	uint32_t payload_size = Responses::read_payload_size_from_buffer(buffer_pointer);
	if (payload_size > 0) {
		append_to_buffer_from_sock(socket, buffer_pointer, payload_size, RESPONSE_HEADER_BYTE_SIZE);
	}
}

void server_sock::read_from_server(boost::asio::ip::tcp::socket& socket,unsigned char* buffer_pointer, int amount_of_bytes_to_read)
{
	boost::asio::read(socket, boost::asio::buffer(buffer_pointer, amount_of_bytes_to_read));
}

void server_sock::append_to_buffer_from_sock(boost::asio::ip::tcp::socket& socket,unsigned char* buffer_pointer, int amount_of_bytes_to_add, int add_starting_from)
{
	boost::asio::read(socket, boost::asio::buffer(buffer_pointer+add_starting_from, amount_of_bytes_to_add));
}

void server_sock::close_connection(boost::asio::ip::tcp::socket &socket)
{
	socket.close();
}

void server_sock::connect_send_and_get(boost::asio::ip::tcp::socket& socket, unsigned char* write_buffer_pointer, int amount_of_bytes_to_send)
{
	server_sock::connect(socket);
	server_sock::write_to_server(socket, write_buffer_pointer, amount_of_bytes_to_send);
	server_sock::read_response_from_server(socket, write_buffer_pointer);
	server_sock::close_connection(socket);
}
