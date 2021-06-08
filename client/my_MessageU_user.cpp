
#include "my_MessageU_user.h"

std::string my_MessageU_user::get_my_name_from_my_file()
{
	std::string first_line;
	std::ifstream file(MY_INFO_FILE);
	std::getline(file, first_line);
	file.close();
	return first_line;
}

void my_MessageU_user::get_my_id_from_my_file(unsigned char* id)
{
	std::string second_line;
	std::ifstream file(MY_INFO_FILE);
	std::getline(file, second_line);
	std::getline(file, second_line);
	unsigned char data[CLIENT_ID_BYTE_SIZE];
	utils::hex_string_to_bytes(second_line.c_str(), (char*)data);
	memcpy(id, data, CLIENT_ID_BYTE_SIZE);
}

std::string my_MessageU_user::get_encoded_base64_key_from_my_file() 
{
	std::string encoded_private_key;
	std::string third_line;
	std::ifstream file(MY_INFO_FILE);
	std::getline(file, third_line);
	std::getline(file, third_line);
	std::getline(file, third_line);
	while (!file.eof()) {
		encoded_private_key.append(third_line);
		encoded_private_key.push_back('\n');
		std::getline(file, third_line);
	}
	encoded_private_key.pop_back();
	file.close();
	return encoded_private_key;
}


my_MessageU_user::my_MessageU_user()
{
	if (utils::have_i_registered()) {
		my_name = get_my_name_from_my_file();
		get_my_id_from_my_file(my_id);
		my_private_key.load_private_key_from_Base64(get_encoded_base64_key_from_my_file());
	}
	else {
		my_name = "";
		my_id;
		my_private_key = Asymmetric_private_key();
	}
}

std::string my_MessageU_user::decrypt_message_using_my_private_key(std::string message)
{
	return my_private_key.decrypt_using_private_key(message);
}

void my_MessageU_user::add_client_name(std::string name)
{
	my_name = name;
}

void my_MessageU_user::add_id(unsigned char* id)
{
	memcpy(my_id,id,CLIENT_ID_BYTE_SIZE);
}

Asymmetric_private_key * my_MessageU_user::get_private_key()
{
	return &my_private_key;
}

std::string my_MessageU_user::get_my_name()
{
	return my_name;
}

unsigned char* my_MessageU_user::get_my_id()
{
	return my_id;
}

void my_MessageU_user::fill_my_info_file()
{
	std::ofstream file(MY_INFO_FILE, std::ios_base::app);
	file << my_name;
	file << '\n';
	file << utils::get_client_id_in_hex(my_id);
	file << '\n';
	file << my_private_key.get_private_key_in_Base64();
	file.close();
}

