#pragma once
#include "utils.h"
#include "Asymmetric_public_key.h"
#include "Symmetric_key_AES_CBC.h"

//class for saving and using the data of the other clients for the projects

class MessageU_user
{
private:
	std::string client_name;
	unsigned char client_id[CLIENT_ID_BYTE_SIZE];
	Asymmetric_public_key *  public_key;
	Symmetric_key_AES_CBC *  symmetric_key;
public:
	MessageU_user(std::string name, unsigned char* client_id_ptr);;
	std::string get_name();
	unsigned char* get_client_id();
	void add_public_key(std::string key);
	void add_symmetric_key(std::string key);
	void add_symmetric_key();
	Asymmetric_public_key* get_user_public_key();
	Symmetric_key_AES_CBC* get_symmetric_key();
	std::string decrypt_message_with_symmetric_key(std::string message);
	bool is_public_key_populated();
	bool is_symmetric_key_populated();
	~MessageU_user();
	MessageU_user(const MessageU_user& R);
	MessageU_user& operator=(const MessageU_user& other);
};

