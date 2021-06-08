import SQLhandling
import ResponseHandling
import utils
import logger

#prepare the reply to the requests

def process_request_100(conn, name, public_key):
    if SQLhandling.check_if_client_with_such_name_exists(conn, name):
        return ResponseHandling.generate_9000_response()
    else:
        client_id = utils.generate_random_16_bytes()
        logger.add_to_log('generated new client id - '+client_id.hex())
        SQLhandling.insert_new_client_to_db(conn, client_id, name, public_key, utils.get_current_time())
        return ResponseHandling.generate_1000_response(client_id)


def process_request_101(conn, client_id):
    if (SQLhandling.check_if_client_with_such_id_exists(conn,client_id) == False):
        return ResponseHandling.generate_9000_response()
    user_list = SQLhandling.get_user_list(conn)
    count = 0
    b = bytearray()
    for user_id, name in user_list:
        logger.add_to_log("adding user - " + name+ " id - " +utils.little_endian_bytes_to_hex(user_id))
        if client_id == user_id:
            continue
        else:
            count += 1
            b.extend(utils.pad_more_bytes(user_id, 16))
            b.extend(utils.string_to_little_endian_bytes_and_pad(name, 255))
    logger.add_to_log('preparing user list,found -'+str(count)+' users')
    return ResponseHandling.generate_1001_response(count, b)


def process_request_102(conn, client_id):
    if SQLhandling.check_if_client_with_such_id_exists(conn, client_id) == False:
        return ResponseHandling.generate_9000_response()
    public_key,= SQLhandling.get_public_key_by_user(conn, client_id)
    public_key = public_key[0]
    logger.add_to_log('preparing client id - '+str(client_id)+' and public key - '+str(public_key))
    b = bytearray()
    b.extend(client_id)
    b.extend(public_key)
    return ResponseHandling.generate_1002_response(b)


def process_request_103(conn, client_id_source, client_id_destination, message_type, message, message_size):
    if SQLhandling.check_if_client_with_such_id_exists(conn, client_id_source) == False or SQLhandling.check_if_client_with_such_id_exists(conn, client_id_destination) == False:
        return ResponseHandling.generate_9000_response()
    else:
        mid = SQLhandling.insert_new_message_to_db_and_return_id(conn, client_id_source, client_id_destination, message_type, message, message_size)
        b = bytearray()
        b.extend(client_id_destination)
        b.extend(utils.int_to_little_endian_bytes_and_pad(mid, 4))
        return ResponseHandling.generate_1003_response(b)


def process_request_104(conn, client_id_destination):
    data = SQLhandling.get_all_messages_for_user(conn, client_id_destination)
    logger.add_to_log('getting all messages sent to user -'+str(client_id_destination)+' data- '+str(data))
    count = 0
    messages_total_size = 0
    b = bytearray()
    for client_id_source, mid, message_type, message in data:
        count += 1
        b.extend(client_id_source)
        b.extend(utils.int_to_little_endian_bytes_and_pad(mid, 4))
        b.extend(utils.int_to_little_endian_bytes(int(message_type)))
        messages_total_size += len(message)
        b.extend(utils.int_to_little_endian_bytes_and_pad(len(message),4))
        b.extend(message)
    logger.add_to_log('found -' + str(count) + ' messages')
    SQLhandling.delete_message_by_destination_user(conn, client_id_destination)
    return ResponseHandling.generate_1004_response(count, messages_total_size, b)
