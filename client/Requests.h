#pragma once
#pragma pack(1)
#include "utils.h"
#include "users_list.h"
#include "my_MessageU_user.h"

const int REQUEST_HEADER_TOTAL_SIZE = 16 + 1 + 1 + 4;
const int REQUEST_100_TOTAL_SIZE = REQUEST_HEADER_TOTAL_SIZE + NAME_BYTE_SIZE + PUBLIC_KEY_BYTE_SIZE;
const int REQUEST_101_TOTAL_SIZE = REQUEST_HEADER_TOTAL_SIZE;
const int REQUEST_102_TOTAL_SIZE = REQUEST_HEADER_TOTAL_SIZE + CLIENT_ID_BYTE_SIZE;
const int REQUEST_103_PAYLOAD_SIZE_WITHOUT_MESSAGE_SIZE = 16 + 1 + 4;
const int REQUEST_103_WITHOUT_MESSAGE_SIZE = REQUEST_HEADER_TOTAL_SIZE + REQUEST_103_PAYLOAD_SIZE_WITHOUT_MESSAGE_SIZE;
const int REQUEST_104_TOTAL_SIZE = REQUEST_HEADER_TOTAL_SIZE;

//static class for all the requests

struct request_header_struct
{
	unsigned char client_id[CLIENT_ID_BYTE_SIZE];
	unsigned char version;
	unsigned char code;
	unsigned char payload_size[PAYLOAD_SIZE_BYTE_SIZE];
};

struct request100_struct
{
	request_header_struct header;
	unsigned char name[NAME_BYTE_SIZE];
	unsigned char public_key[PUBLIC_KEY_BYTE_SIZE];
};

struct request101_struct
{
	request_header_struct header;
};

struct request102_struct
{
	request_header_struct header;
	unsigned char client_id[CLIENT_ID_BYTE_SIZE];
};

struct request103_struct_without_content
{
	request_header_struct header;
	unsigned char client_id[CLIENT_ID_BYTE_SIZE];
	unsigned char message_type;
	unsigned char content_size[PAYLOAD_SIZE_BYTE_SIZE];
};


struct request104_struct
{
	request_header_struct header;
};


enum class RequestCode : uint8_t
{
	REGISTRATION = 100,
	USER_LIST = 101,
	CLIENT_PUBLIC_KEY = 102,
	SEND_MESSAGE = 103,
	GET_MESSAGES = 104
};



 class Requests
{
private:
	Requests();
	static void Prepare_request_100(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user);
	static void Prepare_request_101(unsigned char* buffer, my_MessageU_user* my_user);
	static void Prepare_request_102(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user,unsigned char* destination_id);
	static uint32_t Prepare_request_103(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user,unsigned char* destination_id, std::string message, MessageType message_type);
	static void Prepare_request_104(unsigned char* buffer, my_MessageU_user* my_user);
	static void fill_header(request_header_struct* request_header, unsigned char* client_id, uint8_t code, uint32_t payload_size);
public:
	static void Prepare_Request(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user, RequestCode code);
	static void Prepare_Request(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user, RequestCode code,unsigned char * destination_user_id);
	static uint32_t Prepare_Request(unsigned char* buffer, users_list* my_list, my_MessageU_user* my_user, RequestCode code, unsigned char* destination_user_id, std::string message, MessageType message_type);
};

