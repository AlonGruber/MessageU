#pragma once
#include "Responses.h"
#include "utils.h"

void Responses::digest_response_1000(unsigned char* buffer, my_MessageU_user* my_user)
{
	response1000_struct * response_struct = (response1000_struct*)(buffer);
	my_user->add_id(response_struct->payload_client_id);
	my_user->fill_my_info_file();
	std::cout << "\nRegistration has completed successfully\n" << std::endl;
}


void Responses::digest_response_1001(unsigned char* buffer, users_list* my_user_list)
{
	uint32_t payload_size = Responses::read_payload_size_from_buffer(buffer);
	uint32_t number_of_users = payload_size / (CLIENT_ID_BYTE_SIZE + NAME_BYTE_SIZE);
	response1001_one_user_payload_struct* one_user_struct;
	my_user_list->clear_list();
	for (int i = 0;i < number_of_users;i++) {
		one_user_struct = (response1001_one_user_payload_struct*)(buffer + RESPONSE_HEADER_BYTE_SIZE + (i * RESPONSE1001_ONE_USER_BYTE_SIZE));
		my_user_list->add_user(new MessageU_user(utils::unsigned_char_array_to_string(one_user_struct->payload_name, NAME_BYTE_SIZE), one_user_struct->payload_client_id));
	}
	std::cout <<my_user_list->display_users() << std::endl;
}

void Responses::digest_response_1002(unsigned char* buffer, users_list* my_user_list)
{
	response1002_struct* response_struct = (response1002_struct*)(buffer);
	my_user_list->add_public_key_for_user(response_struct->payload_client_id, utils::unsigned_char_array_to_string(response_struct->payload_public_key,PUBLIC_KEY_BYTE_SIZE));
	std::cout << "Public key received"<< std::endl;
}

void Responses::digest_response_1003(unsigned char* buffer, users_list* my_user_list)
{
	response1003_struct* response_struct = (response1003_struct*)(buffer);
	std::cout << "Message successfully sent to " + my_user_list->get_name_by_id(response_struct->payload_client_id) << std::endl;
}

void Responses::digest_response_1004(unsigned char* buffer, users_list* my_user_list, my_MessageU_user* my_user)
{
	uint32_t payload_size = Responses::read_payload_size_from_buffer(buffer);
	response1004_one_message_payload_without_content_struct* message_header;
	int last_message_size = 0;
	if (payload_size > 0) {
		unsigned char* messages_start = buffer + RESPONSE_HEADER_BYTE_SIZE;
		uint32_t i = 0;
		while ( i < payload_size - 1) {
			message_header = (response1004_one_message_payload_without_content_struct*)(messages_start + i);
			std::cout << "From:";
			std::cout << my_user_list->get_name_by_id(message_header->payload_client_id) << std::endl;
			MessageType message_type = static_cast<MessageType>(message_header->payload_message_type);
			switch (message_type)
			{
			case(MessageType::SYMMETRIC_KEY_REQUEST):
			{
				last_message_size = 0;
				std::cout << "Request for symmetric key" << std::endl;
				break;
			}
			case(MessageType::SYMMETRIC_KEY):
			{
				last_message_size = ENCRYPTED_KEY_SIZE;
				std::string message_symmetric_key = utils::unsigned_char_array_to_string(message_header->payload_message_size + 4, ENCRYPTED_KEY_SIZE);
				std::string decrypted_symmetric_key = my_user->decrypt_message_using_my_private_key(message_symmetric_key);
				my_user_list->add_symmetric_key_for_user(message_header->payload_client_id, decrypted_symmetric_key);
				std::cout << "Symmetric key received" << std::endl;
				break;
			}
			case(MessageType::TEXT_MESSAGE):
			{
				last_message_size = utils::read_message_size_from_buffer(message_header->payload_message_size);
				std::string message = utils::unsigned_char_array_to_string(message_header->payload_message_size + 4, last_message_size);
				try { std::cout << my_user_list->decrypt_user_message(message_header->payload_client_id, message) << std::endl; }
				catch(...){std::cout << "problem decrypting message" << std::endl;}
				break;
			}
			}
			std::cout << "---EOM---" << std::endl;
			i = i + RESPONSE1004_ONE_MESSAGE_HEADER_WITHOUT_MESSAGE_BYTE_SIZE + last_message_size;
		}	
	}
	else {
		std::cout << "no messages for user" << std::endl;
	}
}

void Responses::digest_response(unsigned char* buffer, users_list* my_users_list, my_MessageU_user * my_user)
{
	ResponseCode Response_code = static_cast<ResponseCode>(read_code_from_buffer(buffer));
	switch (Response_code)
	{
	case(ResponseCode::REGISTRATION_COMPLETED):
		{
		digest_response_1000(buffer,my_user);
		break;
		}
	case(ResponseCode::USER_LIST):
		{
		digest_response_1001(buffer, my_users_list);
		break;
		}
	case(ResponseCode::PUBLIC_KEY):
		{
		digest_response_1002(buffer, my_users_list);
		break;
		}
	case(ResponseCode::MESSAGE_SENT):
		{
		digest_response_1003(buffer, my_users_list);
		break;
		}
	case(ResponseCode::MESSAGE_LIST):
		{
		digest_response_1004(buffer, my_users_list,my_user);
		break;
		}
	}
}

uint16_t Responses::read_code_from_buffer(unsigned char* buffer)
{
	response_header_struct* header = (response_header_struct*)buffer;
	uint16_t code;
	code =	(uint16_t)header->code[1] << 8 |
			(uint16_t)header->code[0];
	return code;
}

uint32_t Responses::read_payload_size_from_buffer(unsigned char* buffer)
{
	response_header_struct* header = (response_header_struct*)buffer;
	uint32_t num;
	num =	(uint32_t)header->payload_size[3] << 24 |
			(uint32_t)header->payload_size[2] << 16 |
			(uint32_t)header->payload_size[1] << 8 |
			(uint32_t)header->payload_size[0];
	return num;
}

