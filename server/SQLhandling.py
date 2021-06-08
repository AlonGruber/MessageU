import sqlite3
import logger
import utils

#handling for all SQL requests and pulls

def create_DB_return_connection():
    conn = sqlite3.connect('MessageU.db')
    logger.add_to_log('DB created')
    return conn


def create_table_for_clients(conn):
    cursor = conn.cursor()
    try:
        cursor.execute('''CREATE TABLE IF NOT EXISTS clients(id TEXT(16) NOT NULL PRIMARY KEY, Name TINYTEXT, PublicKey VARCHAR(160), LastSeen DATE)''')
    except:
        logger.add_to_log('cant create client table')
    else:
        logger.add_to_log('client table is in DB')
    conn.commit()
    cursor.close()


def create_table_for_messages(conn):
    cursor = conn.cursor()
    try:
        cursor.execute('''CREATE TABLE IF NOT EXISTS messages(id INTEGER PRIMARY KEY AUTOINCREMENT, ToClient VARCHAR(16), FromClient VARCHAR(16), type CHAR, Content BLOB)''')
    except:
        logger.add_to_log('cant create messages table')
    else:
        logger.add_to_log('message table is in DB')
    conn.commit()
    cursor.close()


def check_if_client_with_such_name_exists(conn, client_name):
    cursor = conn.cursor()
    logger.add_to_log('looking for client - ' + str(client_name)+' in DB')
    cursor.execute('''SELECT name FROM clients WHERE name = (?)''',(client_name,))
    result = cursor.fetchone()
    if result:
        logger.add_to_log('found client with same name in DB')
        return True
    logger.add_to_log('no such client exists in DB')
    return False

def check_if_client_with_such_id_exists(conn, client_id):
    cursor = conn.cursor()
    logger.add_to_log('looking for client - ' + utils.little_endian_bytes_to_hex(client_id) +' in DB')
    cursor.execute('''SELECT name FROM clients WHERE id = (?)''',(client_id,))
    result = cursor.fetchone()
    if result:
        logger.add_to_log('found client with same id in DB')
        return True
    logger.add_to_log('no such client exists in DB')
    return False

def insert_new_client_to_db(conn, uid, name, public_key, last_seen):
    cursor = conn.cursor()
    logger.add_to_log('trying to add to clients table - ' + uid.hex() + ' ' + name + ' ' + public_key.hex() + ' ' + last_seen)
    try:
        cursor.execute('''INSERT INTO clients VALUES(?,?,?,?)''', (uid, name, public_key, last_seen))
    except:
        logger.add_to_log('error while adding entry to clients table')
    else:
        logger.add_to_log('line added to table')
    conn.commit()
    cursor.close()



def update_last_seen_on_client(conn, client_id, new_last_seen):
    cursor = conn.cursor()
    logger.add_to_log('trying to update last seen on client - ' + client_id + ' to - '+ new_last_seen)
    try:
        cursor.execute('''UPDATE clients SET LastSeen = (?) WHERE id = (?)''', (client_id, new_last_seen))
    except:
        logger.add_to_log('error while adding changing last seen on client')
    else:
        logger.add_to_log('last seen updated')
    conn.commit()
    cursor.close()


def insert_new_message_to_db_and_return_id(conn, from_client,to_client, message_type, content, size):
    cursor = conn.cursor()
    logger.add_to_log('trying to add new message to table ,to client - ' + utils.little_endian_bytes_to_hex(to_client) + ' from client - ' + utils.little_endian_bytes_to_hex(from_client)+ ' of type  - ' + str(message_type)+ ' and size - ' + str(size))
    try:
        cursor.execute('''INSERT INTO messages (ToClient,FromClient,type,Content) VALUES(?,?,?,?)''', (to_client,from_client, message_type, content))
    except sqlite3.Error as e:
        logger.add_to_log('failed adding message to table - ' + e.args[0])
    else:
        logger.add_to_log('message added to table ')
    # try:
    #     cursor.execute("SELECT * FROM messages ORDER BY id DESC LIMIT 1")
    #     result = cursor.fetchone()
    id_row = cursor.lastrowid
    conn.commit()
    cursor.close()
    return id_row


def delete_message_by_destination_user(conn, client_id_destination):
    cursor = conn.cursor()
    logger.add_to_log('trying to delete all messages sent to user -'+str(client_id_destination))
    try:
        cursor.execute('''DELETE FROM messages WHERE ToClient = (?)''', (client_id_destination,))
    except sqlite3.Error as e:
        logger.add_to_log('failed deleting messages -' + e.args[0])
    else:
        logger.add_to_log('messages deleted')
    conn.commit()
    cursor.close()


def get_user_list(conn):
    cursor = conn.cursor()
    logger.add_to_log('trying to get the list of all the users')
    try:
        cursor.execute('''SELECT id,Name FROM clients''')
    except:
        logger.add_to_log('could not get list')
    else:
        logger.add_to_log('got the list of all users')
    data = cursor.fetchall()
    return data


def get_public_key_by_user(conn, client_id):
    cursor = conn.cursor()
    logger.add_to_log('trying to get the public key for user - '+str(client_id))
    try:
        cursor.execute('''SELECT PublicKey FROM clients WHERE id = (?)''', (client_id,))
        data = cursor.fetchall()
    except:
        logger.add_to_log('failed getting key')
    else:
        logger.add_to_log('got key for user, key is - '+str(data))
    return data


def get_all_messages_for_user(conn, client_id):
    cursor = conn.cursor()
    logger.add_to_log('trying to get all send to user -'+utils.little_endian_bytes_to_hex(client_id))
    try:
        cursor.execute('''SELECT FromClient,id,type,Content FROM messages WHERE ToClient = (?)''', (client_id,))
        data = cursor.fetchall()
    except sqlite3.Error as e:
        logger.add_to_log('could not get messages - ' + e.args[0])
    else:
        logger.add_to_log('got all messages for user, data - ' + str(data))
    return data
