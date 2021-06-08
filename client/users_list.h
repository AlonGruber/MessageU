#pragma once
#include "utils.h"
#include "MessageU_user.h"
#include "memory.h"

//class for maintaining all the users 

class users_list
{
private:
	std::vector<MessageU_user> user_list_vec;
	unsigned char temp_id[CLIENT_ID_BYTE_SIZE];
	MessageU_user * get_user_by_id(unsigned char* client_id);
	MessageU_user * get_user_by_name(std::string name);
public:
	users_list() = default;
	void add_user(MessageU_user * user);
	void clear_list();
	bool is_user_in_list(std::string name);
	std::string display_users();
	bool does_user_have_public_key(std::string name);
	bool does_user_have_symmetric_key(std::string name);
	bool does_user_have_public_key(unsigned char * id);
	bool does_user_have_symmetric_key(unsigned char * id);
	unsigned char* get_id_by_name(std::string name);
	Symmetric_key_AES_CBC* get_symmetric_key(std::string name);
	Asymmetric_public_key* get_public_key(std::string name);
	void add_public_key_for_user(unsigned char * id,std::string key);
	void add_symmetric_key_for_user(unsigned char* id, std::string key);
	void add_symmetric_key_for_user(std::string name);
	std::string get_name_by_id(unsigned char* id);
	std::string decrypt_user_message(unsigned char * id,std::string msg);
	std::string decrypt_symmetric_key(unsigned char id, std::string msg);
};

