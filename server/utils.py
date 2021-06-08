import datetime
import uuid

# different utilities used in server

def read_port_from_file():
    f = open("port.info", "r")
    port = f.read()
    return port


def little_endian_bytes_to_int(x):
    b = bytearray(x)
    return int.from_bytes(b, "little")


def little_endian_bytes_to_string(x):
    b = bytearray(x)
    return b.decode('utf-8')


def int_to_little_endian_bytes_and_pad(x, destination_length):
    b = x.to_bytes(destination_length, 'little')
    return b

def little_endian_bytes_to_hex(x):
    b=bytearray(x)
    return b.hex()

def int_to_little_endian_bytes(x):
    b = x.to_bytes((x.bit_length() + 7) // 8,'little')
    return b


def string_to_little_endian_bytes_and_pad(x, destination_length):
    b = bytearray()
    b.extend(map(ord,x))
    pad_more_bytes(b,destination_length)
    return b


def string_to_little_endian_bytes(x):
    b = str.encode(x)
    return b

def pad_more_bytes(b,destination_length):
    while len(b) < destination_length:
        b += b'\00'
    return b

def generate_random_16_bytes():
    return uuid.uuid4().bytes


def get_current_time():
    return datetime.datetime.now().strftime("%Y-%m-%d-%H-%M-%S")

def get_name_from_bytes(name):
    b = bytearray()
    for x in range(0,len(name)-1):
        b.append(name[x])
        if not name[x]:
            break;
    return b

def read_content_size_only(x):
    return little_endian_bytes_to_int(x[17:20])
