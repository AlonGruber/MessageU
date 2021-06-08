#include "utils.h"

	 std::string utils::get_text_input(std::string string_to_display,int limit)
	{
		std::string input;
		std::cout << string_to_display << std::endl;
		std::getline(std::cin, input);
		if (input == "") {
			std::cout << "Empty input!"<< std::endl;
			return nullptr;
		}
		else if (input.size() > limit) {
			std::cout << "input is too large" << std::endl;
			return nullptr;
		}
		return input;
	}

	 void utils::display_options()
	 {
		 std::cout << OPTIONS_STRING << std::endl;
	 }

	 int utils::get_option_input()
	{
		int input=0;
		while (true) {
			std::cin >> input;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore();
				std::cout << "Please enter a number" << std::endl;
				continue;
			}
			else if ((!(0 <= input <= 5)) && (input != 51) && (input != 52)) {
				std::cout << "Please enter a number correspodning with the options" << std::endl;
				continue;
			}
			else if (!have_i_registered() && !(input == OPTION_0)&&(input!=OPTION_1)) {
				std::cout << "Please register first, enter 1" << std::endl;
				continue;
			}
			else if (have_i_registered() && input == OPTION_1) {
				std::cout << "You have already registered, enter a different input" << std::endl;
				continue;
			}
			else {
				break;
			}
		}
		std::cin.clear();
		std::cin.ignore();
		return input;
	}
	 std::string utils::get_server_ip() {
		std::ifstream file("server.info");
		std::string server_ip = "";
		std::size_t position;
		char c;
		if (!file.is_open()) {
			std::cout << "Error opening server file" << std::endl;
			return NULL;
		}
		file >> server_ip;
		position = server_ip.find(':');
		file.close();
		return server_ip.substr(0, position);
	}
	 int utils::get_server_port() {
		std::ifstream file("server.info");
		std::string file_line;
		int server_port;
		file >> file_line;
		file.close();
		return std::stoi(file_line.substr(file_line.find(":") + 1));
	}


	 std::string utils::get_client_id_in_hex(BYTE* client_id)
	 {
		 std::vector<unsigned char> v;
		 for (int i = 0;i < 16;i++) {
			 v.push_back(client_id[i]);
		 }
		 std::string res;
		 boost::algorithm::hex(v.begin(), v.end(), back_inserter(res));

		 std::ostringstream ss;

		 ss << std::hex << std::uppercase << std::setfill('0');
		 std::for_each(v.cbegin(), v.cend(), [&](int c) { ss << std::setw(2) << c; });

		 std::string result = ss.str();
		 return result;

	 }

	 int utils::char2int(unsigned char input)
	 {
		 if (input >= '0' && input <= '9')
			 return input - '0';
		 if (input >= 'A' && input <= 'F')
			 return input - 'A' + 10;
		 if (input >= 'a' && input <= 'f')
			 return input - 'a' + 10;
		 throw std::invalid_argument("Invalid input string");
	 };





	 void utils::hex_string_to_bytes(const char* src, char* target)
	 {
		 while (*src && src[1])
		 {
			 *(target++) = char2int(*src) * 16 + char2int(src[1]);
			 src += 2;
		 }
	 }

	 
		 



	 
	 void utils::swap_endian(unsigned char * buffer,int buffer_size){
		 unsigned char temp;
		 for (int i = 0;i < buffer_size/2;i++) {
			 temp = buffer[i];
			 buffer[i] = buffer[buffer_size - 1 - i];
			 buffer[buffer_size - 1 - i] = temp;
		 }
	 }

	 void utils::copy_memory(unsigned char * to, unsigned char * from,int size) {
		 memcpy(to,from, size);
	 }

	 void utils::string_to_unsigned_char_arr(std::string string,unsigned char * copy_to,uint32_t copy_to_size)
	 {
		 uint8_t string_size = string.length();
		 if (string_size > copy_to_size) {
			 std::cout << "buffer not big enough" << std::endl;
		 }
		 std::copy(string.begin(), string.end(), copy_to);
	 }

	 void utils::uint32_to_unsigned_char_size_4(uint32_t number, unsigned char * copy_to) {
		 memcpy(copy_to, &number, PAYLOAD_SIZE_BYTE_SIZE);
	 }

	 std::string utils::unsigned_char_array_to_string(unsigned char* array, int array_size)
	 {
		 std::string str(reinterpret_cast<char const*>(array), array_size);
		 for (auto x : str) {
			 x = (unsigned char)x;
		 }
		 return str;
	 }

	 std::string utils::unsigned_char_array_to_string_this_time_really(unsigned char* array, int array_size) 
	 {
		 std::string str(reinterpret_cast<char const*>(array), array_size);
		 const BYTE* str1 = reinterpret_cast<const BYTE*> (&str);
		 return std::string (str1, str1 + array_size);
	 }

	 bool utils::check_if_file_exists(std::string name)
	 {
		 return (boost::filesystem::exists(name));
	 }

	 


	 bool utils::have_i_registered()
	 {
		 return(utils::check_if_file_exists(MY_INFO_FILE));
	 }




	 uint32_t utils::read_message_size_from_buffer(unsigned char* message_size)
	 {
		 uint32_t num;
		 num = (uint32_t)message_size[3] << 24 |
			 (uint32_t)message_size[2] << 16 |
			 (uint32_t)message_size[1] << 8 |
			 (uint32_t)message_size[0];
		 return num;
	 }
	 


	 
	
