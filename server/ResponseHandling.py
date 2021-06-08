import utils
import logger

SERVER_VERSION = 2
CODE_1000 = 1000
CODE_1001 = 1001
CODE_1002 = 1002
CODE_1003 = 1003
CODE_1004 = 1004
CODE_9000 = 9000

CODE_1000_PAYLOAD_SIZE = 16
CODE_1001_ONE_USER_SIZE = 16 + 255
CODE_1002_PAYLOAD_SIZE = 16 + 160
CODE_1003_PAYLOAD_SIZE = 16 + 4
CODE_1004_ONE_MESSAGE_SIZE = 16 + 4 + 1 + 4

EMPTY_PAYLOAD = b'\x00\x00\x00\x00'

#generate response payload data and headers

def generate_response_header(code):
    b = bytearray()
    b.extend(utils.int_to_little_endian_bytes(SERVER_VERSION))
    b.extend(utils.int_to_little_endian_bytes(code))
    logger.add_to_log('generating response header bytearray, current b -'+str(b))
    return b


def generate_1000_response(client_id):
    b = generate_response_header(CODE_1000)
    b.extend(utils.int_to_little_endian_bytes_and_pad(CODE_1000_PAYLOAD_SIZE, 4))
    b.extend(client_id)
    logger.add_to_log('adding payload size and client id to b-'+str(b))
    return b


def generate_1001_response(user_count, data):
    b = generate_response_header(CODE_1001)
    b.extend(utils.int_to_little_endian_bytes_and_pad(user_count * CODE_1001_ONE_USER_SIZE, 4))
    b.extend(data)
    logger.add_to_log('adding payload size and user list to b-' + str(b))
    return b


def generate_1002_response(data):
    b = generate_response_header(CODE_1002)
    b.extend(utils.int_to_little_endian_bytes_and_pad(CODE_1002_PAYLOAD_SIZE, 4))
    b.extend(data)
    logger.add_to_log('adding payload size, user id and public key to b-' + str(b))
    return b


def generate_1003_response(data):
    b = generate_response_header(CODE_1003)
    b.extend(utils.int_to_little_endian_bytes_and_pad(CODE_1003_PAYLOAD_SIZE, 4))
    b.extend(data)
    logger.add_to_log('adding client id and message id to b-' + str(b))
    return b


def generate_1004_response(msg_count, msg_additional_bytes, data):
    b = generate_response_header(CODE_1004)
    b.extend(utils.int_to_little_endian_bytes_and_pad(CODE_1004_ONE_MESSAGE_SIZE * msg_count + msg_additional_bytes,4))
    b.extend(data)
    logger.add_to_log('adding payload size all messages sent to user to b-' + str(b))
    return b


def generate_9000_response():
    b = generate_response_header(CODE_9000)
    b.extend(EMPTY_PAYLOAD)
    logger.add_to_log('adding code 9000 to b-' + str(b))
    return b
