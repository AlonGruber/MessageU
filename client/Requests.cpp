#include "Requests.h"
#include "utils.h"

void Requests::fill_header(request_header_struct* request_header,unsigned char * client_id, uint8_t code, uint32_t payload_size){
	memcpy(request_header->client_id,client_id, CLIENT_ID_BYTE_SIZE);
	request_header->version = CLIENT_VERSION;
	request_header->code = code;
	utils::uint32_to_unsigned_char_size_4(payload_size, request_header->payload_size);

}

void Requests::Prepare_request_100(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user)
{
	request100_struct* request_struct = (request100_struct*)buffer;
	fill_header(&request_struct->header, my_user->get_my_id(), static_cast<uint8_t>(RequestCode::REGISTRATION), REQUEST_100_TOTAL_SIZE-REQUEST_HEADER_TOTAL_SIZE);
	utils::string_to_unsigned_char_arr(my_user->get_my_name(), request_struct->name, NAME_BYTE_SIZE);
	Asymmetric_public_key *  key = new Asymmetric_public_key(my_user->get_private_key());
	utils::string_to_unsigned_char_arr(key->get_raw_key_in_string(), request_struct->public_key,PUBLIC_KEY_BYTE_SIZE);
}

void Requests::Prepare_request_101(unsigned char* buffer, my_MessageU_user* my_user)
{
	request101_struct* request_struct = (request101_struct*)buffer;
	fill_header(&request_struct->header, my_user->get_my_id(), static_cast<uint8_t>(RequestCode::USER_LIST), 0);
}

void Requests::Prepare_request_102(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user,unsigned char* destination_id)
{
	request102_struct* request_struct = (request102_struct*)buffer;
	fill_header(&request_struct->header, my_user->get_my_id(), static_cast<uint8_t>(RequestCode::CLIENT_PUBLIC_KEY), REQUEST_102_TOTAL_SIZE- REQUEST_HEADER_TOTAL_SIZE);
	memcpy(request_struct->client_id, destination_id,CLIENT_ID_BYTE_SIZE);
}

uint32_t Requests::Prepare_request_103(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user, unsigned char* destination_user_id, std::string message, MessageType message_type)
{
	request103_struct_without_content* request_struct = (request103_struct_without_content*)buffer;
	memcpy(request_struct->client_id, destination_user_id,CLIENT_ID_BYTE_SIZE);
	request_struct->message_type = static_cast<uint8_t>(message_type);
	switch (message_type)
	{
	case(MessageType::SYMMETRIC_KEY_REQUEST):
	{
		fill_header(&request_struct->header, my_user->get_my_id(), static_cast<uint8_t>(RequestCode::SEND_MESSAGE), REQUEST_103_WITHOUT_MESSAGE_SIZE- REQUEST_HEADER_TOTAL_SIZE);
		utils::uint32_to_unsigned_char_size_4(0, request_struct->content_size);
		return REQUEST_103_WITHOUT_MESSAGE_SIZE;
		break;
	}
	case(MessageType::SYMMETRIC_KEY):
	{
		if (!my_list->does_user_have_symmetric_key(my_list->get_name_by_id(destination_user_id))) {
			my_list->add_symmetric_key_for_user(my_list->get_name_by_id(destination_user_id));
		}
		std::string encrypted_key = my_list->get_public_key(my_list->get_name_by_id(destination_user_id))->encrypt_using_public_key(my_list->get_symmetric_key(my_list->get_name_by_id(destination_user_id))->get_symmetric_key_string());
		fill_header(&request_struct->header, my_user->get_my_id(), static_cast<uint8_t>(RequestCode::SEND_MESSAGE), REQUEST_103_WITHOUT_MESSAGE_SIZE- REQUEST_HEADER_TOTAL_SIZE + encrypted_key.size());
		utils::uint32_to_unsigned_char_size_4(encrypted_key.size(), request_struct->content_size);
		utils::string_to_unsigned_char_arr(encrypted_key, (unsigned char *)request_struct + REQUEST_103_WITHOUT_MESSAGE_SIZE, encrypted_key.size());
		return REQUEST_103_WITHOUT_MESSAGE_SIZE+encrypted_key.size();
		break;
	}
	case(MessageType::TEXT_MESSAGE):
	{
		std::string encrypted_message = my_list->get_symmetric_key(my_list->get_name_by_id(destination_user_id))->encrypt_using_key(message);
		fill_header(&request_struct->header, my_user->get_my_id(), static_cast<uint8_t>(RequestCode::SEND_MESSAGE), REQUEST_103_WITHOUT_MESSAGE_SIZE- REQUEST_HEADER_TOTAL_SIZE + encrypted_message.size());
		utils::uint32_to_unsigned_char_size_4(encrypted_message.size(), request_struct->content_size);
		utils::string_to_unsigned_char_arr(encrypted_message, (unsigned char*)request_struct + REQUEST_103_WITHOUT_MESSAGE_SIZE, encrypted_message.size());
		return REQUEST_103_WITHOUT_MESSAGE_SIZE+encrypted_message.size();
		break;
	}
	}
}

void Requests::Prepare_request_104(unsigned char* buffer, my_MessageU_user* my_user)
{
	request104_struct* request_struct = (request104_struct*)buffer;
	fill_header(&request_struct->header, my_user->get_my_id(), static_cast<uint8_t>(RequestCode::GET_MESSAGES), REQUEST_104_TOTAL_SIZE-REQUEST_HEADER_TOTAL_SIZE);
}

void Requests::Prepare_Request(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user, RequestCode code)
{
	switch (code)
	{
	case(RequestCode::REGISTRATION):
	{
		Prepare_request_100(buffer, my_list, my_user);
		break;
	}
	case(RequestCode::USER_LIST):
	{
		Prepare_request_101(buffer, my_user);
		break;
	}
	case(RequestCode::GET_MESSAGES):
	{
		Prepare_request_104(buffer, my_user);
		break;
	}
	}
}

void Requests::Prepare_Request(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user, RequestCode code,unsigned char * destination_id)
{
	switch (code)
	{
	case(RequestCode::CLIENT_PUBLIC_KEY):
	{
		Prepare_request_102(buffer, my_list, my_user,destination_id);
		break;
	}
	}
}

uint32_t Requests::Prepare_Request(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user, RequestCode code, unsigned char* destination_user_id, std::string message, MessageType message_type)
{
	switch (code)
	{
	case(RequestCode::SEND_MESSAGE):
	{
		return Prepare_request_103(buffer, my_list, my_user,destination_user_id,message,message_type);
		break;
	}
	}
}
