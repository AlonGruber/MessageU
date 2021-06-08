#pragma once
#pragma pack(1)
#include "utils.h"
#include "users_list.h"
#include "my_MessageU_user.h"

const int RESPONSE_HEADER_BYTE_SIZE = 1 + 2 + 4;
const int RESPONSE1000_BYTE_SIZE = RESPONSE_HEADER_BYTE_SIZE + 16;
const int RESPONSE1001_WITHOUT_LIST_BYTE_SIZE = RESPONSE_HEADER_BYTE_SIZE;
const int RESPONSE1001_ONE_USER_BYTE_SIZE = 16 + 255;
const int RESPONSE1002_BYTE_SIZE = RESPONSE_HEADER_BYTE_SIZE + 16 + 160;
const int RESPONSE1003_BYTE_SIZE = RESPONSE_HEADER_BYTE_SIZE + 16 + 4;
const int RESPONSE1004_WITHOUT_MESSAGES_BYTE_SIZE = RESPONSE_HEADER_BYTE_SIZE;
const int RESPONSE1004_ONE_MESSAGE_HEADER_WITHOUT_MESSAGE_BYTE_SIZE = 16 + 4 + 1 + 4;
const int RESPONSE1009_BYTE_SIZE = RESPONSE_HEADER_BYTE_SIZE;

//static class for all the responses

struct response_header_struct
{
	unsigned char version;
	unsigned char code[CODE_BYTE_SIZE];
	unsigned char payload_size[PAYLOAD_SIZE_BYTE_SIZE];
};

struct response1000_struct
{
	response_header_struct response_header;
	unsigned char payload_client_id[CLIENT_ID_BYTE_SIZE];
};

struct response1001_one_user_payload_struct
{
	unsigned char payload_client_id[CLIENT_ID_BYTE_SIZE];
	unsigned char payload_name[NAME_BYTE_SIZE];
};


struct response1002_struct
{
	response_header_struct response_header;
	unsigned char payload_client_id[CLIENT_ID_BYTE_SIZE];
	unsigned char payload_public_key[PUBLIC_KEY_BYTE_SIZE];
};

struct response1003_struct
{
	response_header_struct response_header;
	unsigned char payload_client_id[CLIENT_ID_BYTE_SIZE];
	unsigned char payload_message_id[MESSAGE_ID_BYTE_SIZE];
};

struct response1004_one_message_payload_without_content_struct
{
	unsigned char payload_client_id[CLIENT_ID_BYTE_SIZE];
	unsigned char payload_message_id[MESSAGE_ID_BYTE_SIZE];
	unsigned char payload_message_type;
	unsigned char payload_message_size[MESSAGE_SIZE_BYTE_SIZE];
};

enum class ResponseCode : uint16_t
{
	REGISTRATION_COMPLETED = 1000,
	USER_LIST = 1001,
	PUBLIC_KEY = 1002,
	MESSAGE_SENT = 1003,
	MESSAGE_LIST = 1004
};


class Responses
{
private:
	Responses();
	static void digest_response_1000(unsigned char* buffer, my_MessageU_user* my_user);
	static void digest_response_1001(unsigned char* buffer, users_list* my_user_list);
	static void digest_response_1002(unsigned char* buffer, users_list* my_user_list);
	static void digest_response_1003(unsigned char* buffer, users_list* my_user_list);
	static void digest_response_1004(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user);
public:
	static void digest_response(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user);
	static uint16_t read_code_from_buffer(unsigned char* buffer);
	static uint32_t read_payload_size_from_buffer(unsigned char* buffer);
};

