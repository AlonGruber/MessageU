#pragma once
#include "Asymmetric_private_key.h"
#include "utils.h"

//class for keeping and using all the data on my user 

class my_MessageU_user
{
private:
	std::string my_name;
	unsigned char my_id[CLIENT_ID_BYTE_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	Asymmetric_private_key my_private_key;
	
	void get_my_id_from_my_file(unsigned char* id);
	std::string get_my_name_from_my_file();
	std::string get_encoded_base64_key_from_my_file();

public:
	my_MessageU_user();

	std::string decrypt_message_using_my_private_key(std::string message);

	void add_client_name(std::string name);
	void add_id(unsigned char * id);

	Asymmetric_private_key * get_private_key();
	std::string get_my_name();
	unsigned char* get_my_id();

	void fill_my_info_file();
};

