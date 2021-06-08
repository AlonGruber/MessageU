#pragma once

#include "boost/algorithm/hex.hpp"
#include <string>
#include <fstream>
#include "IO.h"
#include <iostream>
#include <climits>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>

//class for all the utilities used in project

const std::string BOOT_MESSAGE = "MessageU client at your service.";
const std::string OPTIONS_STRING = "1)Register\n2)Request for client list\n3)Request for public key\n4)Request for waiting messages\n5)Send a text message\n51)Send a request for symmetric key\n52)Send your symmetric key\n0)Exit client";
const std::string MY_INFO_FILE = "me.info";

const int OPTION_1 = 1;
const int OPTION_2 = 2;
const int OPTION_3 = 3;
const int OPTION_4 = 4;
const int OPTION_5 = 5;
const int OPTION_51 = 51;
const int OPTION_52 = 52;
const int OPTION_0 = 0;

const int CLIENT_ID_BYTE_SIZE = 16;
const int PAYLOAD_SIZE_BYTE_SIZE = 4;
const int NAME_BYTE_SIZE = 255;
const int PUBLIC_KEY_BYTE_SIZE = 160;
const int VERSION_BYTE_SIZE = 1;
const int CODE_BYTE_SIZE = 2;
const int MESSAGE_ID_BYTE_SIZE = 4;
const int MESSAGE_TYPE_BYTE_SIZE = 1;
const int MESSAGE_SIZE_BYTE_SIZE = 4;
const int PAYLOAD_START_AFTER_HEADER_IN_RESPONSE = 7;

enum class MessageType : uint8_t
{
	SYMMETRIC_KEY_REQUEST = 1,
	SYMMETRIC_KEY = 2,
	TEXT_MESSAGE = 3
};


const int ENCRYPTED_KEY_SIZE = 128;
const uint8_t CLIENT_VERSION = 1;
const int MAX_MESSAGE_SIZE = 640000;
const std::string MEANINGLESS_16BYTE = "0000000000000000";

	class utils
	{
	public:
		static void display_options();
		static int get_option_input();
		static std::string get_text_input(std::string string_to_display,int limit);
		static std::string get_server_ip();
		static int get_server_port();
		static std::string get_client_id_in_hex(BYTE* client_id);
		static void swap_endian(unsigned char * buffer,int buffer_size);
		static void copy_memory(unsigned char* to, unsigned char* from, int size);
		static void string_to_unsigned_char_arr(std::string string,unsigned char * copy_to,uint32_t copy_to_size);
		static void uint32_to_unsigned_char_size_4(uint32_t number, unsigned char * copy_to);
		static std::string unsigned_char_array_to_string(unsigned char* array, int array_size);
		static bool check_if_file_exists(std::string name);
		static bool have_i_registered();
		static int char2int(unsigned char input);
		static void hex_string_to_bytes(const char* src, char* target);
		static uint32_t read_message_size_from_buffer(unsigned char* message_size);
		static std::string unsigned_char_array_to_string_this_time_really(unsigned char* array, int array_size);
	};
