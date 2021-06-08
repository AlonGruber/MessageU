
#include <memory>
#include <iostream>
#include <stdlib.h>
#include <cryptlib.h>
#include "Asymmetric_private_key.h"
#include "Asymmetric_public_key.h"
#include "Symmetric_key_AES_CBC.h"
#include "Requests.h"
#include "users_list.h"
#include "Responses.h"
#include "server_sock.h"
#include "utils.h"


int main()
{
	unsigned char * communication_buffer = new unsigned char(MAX_MESSAGE_SIZE);
	my_MessageU_user * my_user = new my_MessageU_user();
	users_list * my_user_list = new users_list();

	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket socket(io_service);
	boost::system::error_code error;

	std::cout << BOOT_MESSAGE << std::endl;

	while (true) {
		utils::display_options();
		switch (utils::get_option_input()) {
		case(OPTION_1):
		{
			my_user->add_client_name(utils::get_text_input("Enter your username", NAME_BYTE_SIZE) + '\0');
			Requests::Prepare_Request(communication_buffer,my_user_list,my_user,RequestCode::REGISTRATION);
			server_sock::connect_send_and_get(socket,communication_buffer, REQUEST_100_TOTAL_SIZE);
			Responses::digest_response(communication_buffer, my_user_list, my_user);
			break;
		}
		case(OPTION_2):
		{
			Requests::Prepare_Request(communication_buffer, my_user_list, my_user, RequestCode::USER_LIST);
			server_sock::connect_send_and_get(socket,communication_buffer,REQUEST_101_TOTAL_SIZE );
			Responses::digest_response(communication_buffer, my_user_list, my_user);
			break;
		}
		case(OPTION_3):
		{
			std::string required_user = utils::get_text_input("Enter the requred user name", NAME_BYTE_SIZE);
			if (!my_user_list->is_user_in_list(required_user)){break;}
			Requests::Prepare_Request(communication_buffer, my_user_list, my_user, RequestCode::CLIENT_PUBLIC_KEY,my_user_list->get_id_by_name(required_user));
			server_sock::connect_send_and_get(socket, communication_buffer,REQUEST_102_TOTAL_SIZE);
			Responses::digest_response(communication_buffer, my_user_list, my_user);
			break;
		}
		case(OPTION_4):
		{
			Requests::Prepare_Request(communication_buffer, my_user_list, my_user, RequestCode::GET_MESSAGES);
			server_sock::connect_send_and_get(socket, communication_buffer,REQUEST_104_TOTAL_SIZE);
			Responses::digest_response(communication_buffer, my_user_list, my_user);
			break;
		}
		case(OPTION_5):
		{
			std::string user_to_send = utils::get_text_input("To who do you want to send a message?",NAME_BYTE_SIZE);
			if (!(my_user_list->is_user_in_list(user_to_send))) { std::cout << "no such user exists" << std::endl;break; }
			if (!(my_user_list->does_user_have_symmetric_key(user_to_send))) { std::cout << "don't have symmetric key for user" << std::endl; break;}
			std::string message = utils::get_text_input("Enter the message you would like to send-", MAX_MESSAGE_SIZE - REQUEST_104_TOTAL_SIZE);
			uint32_t message_size = Requests::Prepare_Request(communication_buffer, my_user_list, my_user, RequestCode::SEND_MESSAGE, my_user_list->get_id_by_name(user_to_send),message,MessageType::TEXT_MESSAGE);
			server_sock::connect_send_and_get(socket, communication_buffer, message_size);
			Responses::digest_response(communication_buffer, my_user_list, my_user);
			break;
		}
		case(OPTION_51):
		{
			std::string user_to_send = utils::get_text_input("To who would you like to send a symmetric key request?", NAME_BYTE_SIZE);
			if (!(my_user_list->is_user_in_list(user_to_send))) {std::cout << "no such user exists" << std::endl;break;}
			uint32_t request_size = Requests::Prepare_Request(communication_buffer, my_user_list, my_user, RequestCode::SEND_MESSAGE, my_user_list->get_id_by_name(user_to_send),"",MessageType::SYMMETRIC_KEY_REQUEST);
			server_sock::connect_send_and_get(socket, communication_buffer, request_size);
			Responses::digest_response(communication_buffer, my_user_list, my_user);
			break;
		}
		case(OPTION_52):
		{
			std::string user_to_send = utils::get_text_input("To who would you like to send your symmetric key?", NAME_BYTE_SIZE);
			if (!(my_user_list->is_user_in_list(user_to_send))) {std::cout << "no such user exists" << std::endl;break;}
			if (!(my_user_list->does_user_have_public_key(user_to_send))) {std::cout << "don't have public key for user" << std::endl;break;}
			uint32_t request_size = Requests::Prepare_Request(communication_buffer, my_user_list, my_user, RequestCode::SEND_MESSAGE, my_user_list->get_id_by_name(user_to_send), "", MessageType::SYMMETRIC_KEY);
			server_sock::connect_send_and_get(socket, communication_buffer, request_size);
			Responses::digest_response(communication_buffer, my_user_list, my_user);
		break;
		}
		case(OPTION_0):
		{
			return 0;
			break;
		}
		}
	}
}
