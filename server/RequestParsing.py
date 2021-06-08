import struct
import utils
import logger

# first block of processing - receiving packets and unpacking them
# all data beyond this point is reversed back to big endian and is stored and used in that way

HEADER_SIZE = 16 + 1 + 1 + 4  # total size of header as defined in protocol documentation
PAYLOAD_WITHOUT_MESSAGE_SIZE = 16 + 1 + 4

def read_x_bytes_from_socket(sock, x):
    data = sock.recv(x)
    return data


def read_request_header_from_socket(sock):  # parsing of the header according to sizes of fields
    data = read_x_bytes_from_socket(sock, HEADER_SIZE)  # fields are returned reversed since sending is in little endian
    client_id, version, code, payload_size = struct.unpack('<16s1B1B4s', data)  # and processing is in big endian
    logger.add_to_log('Request received! header data - client_id - ' + utils.little_endian_bytes_to_hex(client_id) + ' version - ' + str(version) + ' code- ' + str(code) + ' payload size - ' + str(utils.little_endian_bytes_to_int(payload_size)))
    return client_id,  str(version),  str(code), utils.little_endian_bytes_to_int(payload_size)


def read_payload_from_socket(sock, payload_size): #read payload after payload size is read from header
    if payload_size == 0:
        pass
    payload = read_x_bytes_from_socket(sock, payload_size)
    logger.add_to_log('Reading payload from socket! byte count - '+str(payload_size))
    return payload


def unpack_code_100_from_data(data):  # payload unpacking for code 100
    name, public_key = struct.unpack('<255s160s', data)
    name = utils.get_name_from_bytes(name)
    logger.add_to_log('unpacking payload data! name - ' + utils.little_endian_bytes_to_string(name) + ' public key - ' + str(public_key.hex()))
    return utils.little_endian_bytes_to_string(name),public_key


def unpack_code_102_from_data(data):  # payload unpacking for code 102
    client_id, = struct.unpack('<16s', data)
    logger.add_to_log('unpacking payload data! client id - ' + utils.little_endian_bytes_to_hex(client_id))
    return client_id


def unpack_code_103_from_data(data):  # payload unpacking for code 103
    message_size_in_int = utils.read_content_size_only(data)
    client_destination_id, message_type, message_size,message_content = struct.unpack('<16s1B4s'+str(message_size_in_int)+'s', data)
    logger.add_to_log('unpacking payload data! client_destination_id - ' + utils.little_endian_bytes_to_hex(client_destination_id) + ' message_type - ' + str(message_type) + ' message_size - ' + str(message_size_in_int))
    return client_destination_id, message_type, message_size_in_int, message_content

