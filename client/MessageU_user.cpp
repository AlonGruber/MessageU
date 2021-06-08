#include "MessageU_user.h"

MessageU_user::MessageU_user(std::string name, unsigned char* client_id_ptr)
{
	client_name = name;
	client_name.erase(std::remove(client_name.begin(), client_name.end(), '\0'), client_name.end());
	memcpy(client_id, client_id_ptr, CLIENT_ID_BYTE_SIZE);
	public_key = NULL;
	symmetric_key = NULL;
}

void MessageU_user::add_public_key(std::string key)
{
	public_key = new Asymmetric_public_key(key);
}

void MessageU_user::add_symmetric_key(std::string key)
{
	symmetric_key = new Symmetric_key_AES_CBC(key);
}

void MessageU_user::add_symmetric_key()
{
	symmetric_key = new Symmetric_key_AES_CBC();
}

Asymmetric_public_key* MessageU_user::get_user_public_key()
{
	return  public_key;
}

Symmetric_key_AES_CBC* MessageU_user::get_symmetric_key()
{
	return symmetric_key;
}


std::string MessageU_user::decrypt_message_with_symmetric_key(std::string message)
{
	return symmetric_key->decrypt_using_key(message);
}

std::string MessageU_user::get_name()
{
	return client_name;
}

unsigned char* MessageU_user::get_client_id()
{
	return client_id;
}

bool MessageU_user::is_public_key_populated()
{
	return (public_key != NULL);
}

bool MessageU_user::is_symmetric_key_populated()
{
	return (symmetric_key != NULL);
}

MessageU_user::~MessageU_user()
{
	if (public_key != NULL) {
		delete(public_key);
		public_key = NULL;
	}
	if (symmetric_key != NULL) {
		delete(symmetric_key);
		symmetric_key = NULL;
	}
}

MessageU_user::MessageU_user(const MessageU_user& R)
{
	client_name = R.client_name;
	memcpy(client_id, R.client_id, CLIENT_ID_BYTE_SIZE);
	if (R.public_key != NULL) {
		public_key = new Asymmetric_public_key(*R.public_key);
	}
	else {
		public_key = NULL;
	}
	if (R.symmetric_key != NULL) {
		symmetric_key = new Symmetric_key_AES_CBC(*R.symmetric_key);
	}
	else {
		symmetric_key = NULL;
	}
}

MessageU_user& MessageU_user::operator=(const MessageU_user& other)
{
	client_name = other.client_name;
	memcpy(client_id, other.client_id, CLIENT_ID_BYTE_SIZE);
	public_key = other.public_key;
	symmetric_key = other.symmetric_key;
	return *this;
}


