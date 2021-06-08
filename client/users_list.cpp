#include "users_list.h"


MessageU_user * users_list::get_user_by_id(unsigned char* client_id)
{
	std::string client_id_string = utils::unsigned_char_array_to_string(client_id, CLIENT_ID_BYTE_SIZE);
	for (auto& user : user_list_vec) {
		std::string list_client_id = utils::unsigned_char_array_to_string(user.get_client_id(), CLIENT_ID_BYTE_SIZE);
		if (list_client_id == client_id_string)
			return &user;
	}
}

MessageU_user * users_list::get_user_by_name(std::string name)
{
	for (auto& user : user_list_vec) {
		if (user.get_name() == name)
			return &user;
	}
}


void users_list::add_user(MessageU_user * user)
{
	user_list_vec.push_back(*user);
}

void users_list::clear_list()
{
	user_list_vec.clear();
}

bool users_list::is_user_in_list(std::string name)
{
	for (auto user : user_list_vec) {
		if (user.get_name() ==name)
			return true;
	}
	return false;
}


std::string users_list::display_users()
{
	std::string list;
	for(auto user : user_list_vec) {
		list += user.get_name();
		list += '\t';
	}
	return list;
}

bool users_list::does_user_have_public_key(std::string name)
{
	MessageU_user * user = get_user_by_name(name);
	return user->is_public_key_populated();
}

bool users_list::does_user_have_symmetric_key(std::string name)
{
	MessageU_user * user = get_user_by_name(name);
	return user->is_symmetric_key_populated();
}

bool users_list::does_user_have_public_key(unsigned char* id)
{
	MessageU_user * user = get_user_by_id(id);
	return user->is_public_key_populated();
}

bool users_list::does_user_have_symmetric_key(unsigned char* id)
{
	MessageU_user * user = get_user_by_id(id);
	return user->is_symmetric_key_populated();
}

unsigned char* users_list::get_id_by_name(std::string name) {
	MessageU_user * user = get_user_by_name(name);
	return user->get_client_id();
}

Symmetric_key_AES_CBC* users_list::get_symmetric_key(std::string name)
{
	MessageU_user * user = get_user_by_name(name);
	return user->get_symmetric_key();
}

Asymmetric_public_key* users_list::get_public_key(std::string name)
{
	MessageU_user * user = get_user_by_name(name);
	return user->get_user_public_key();
}

void users_list::add_public_key_for_user(unsigned char * id,std::string key)
{
	MessageU_user * user = get_user_by_id(id);
	user->add_public_key(key);
}

void users_list::add_symmetric_key_for_user(unsigned char* id, std::string key )
{
	MessageU_user * user = get_user_by_id(id);
	user->add_symmetric_key(key);
}

void users_list::add_symmetric_key_for_user(std::string name)
{
	MessageU_user* user = get_user_by_name(name);
	user->add_symmetric_key();
}

std::string users_list::get_name_by_id(unsigned char* id)
{
	MessageU_user * user = get_user_by_id(id);
	return user->get_name();
}

std::string users_list::decrypt_user_message(unsigned char * id,std::string message)
{
	MessageU_user * user = get_user_by_id(id);
	return user->decrypt_message_with_symmetric_key(message);
}
