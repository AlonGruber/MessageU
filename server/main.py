import socket
import utils
import select
import queue
import RequestParsing
import SQLhandling
import RequestToResponse
import logger


def main():
    DB_connection = SQLhandling.create_DB_return_connection()
    SQLhandling.create_table_for_clients(DB_connection)
    SQLhandling.create_table_for_messages(DB_connection)
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setblocking(False)
    port = utils.read_port_from_file()
    server.bind(('localhost', int(port)))
    server.listen(5)
    inputs = [server]
    output = []
    queues = {}

    while inputs:
        readable_socks, writeable_socks, exception_socks = select.select(inputs, output, queues)    #selector for working with multiple sockets
        for sock in readable_socks:
            if sock is server: #this part is accessed the first time a socket request is made
                client_connection, client_address = sock.accept()
                client_connection.setblocking(False)
                inputs.append(client_connection)
                queues[client_connection] = queue.Queue()
            else: #request parsing for the server
                client_id, version, code, payload_size = RequestParsing.read_request_header_from_socket(sock)
                if payload_size is not None: #if payload is not empty
                    payload = RequestParsing.read_payload_from_socket(sock, payload_size)
                b = None #b is empty at first, and filled according to responses
                if code == '100':
                    name, public_key = RequestParsing.unpack_code_100_from_data(payload)
                    b = RequestToResponse.process_request_100(DB_connection, name, public_key)
                elif code == '101':
                    b = RequestToResponse.process_request_101(DB_connection, client_id)
                elif code == '102':
                    client_id = RequestParsing.unpack_code_102_from_data(payload)
                    b = RequestToResponse.process_request_102(DB_connection, client_id)
                elif code == '103':
                    client_destination_id, message_type, message_size, message_content = RequestParsing.unpack_code_103_from_data(payload)
                    b = RequestToResponse.process_request_103(DB_connection, client_id, client_destination_id,message_type, message_content, message_size)
                elif code == '104':
                    b = RequestToResponse.process_request_104(DB_connection, client_id)
                if client_id:
                    queues[sock].put(b) #now that we have data to send, add it to send queue
                    if sock not in output:
                        output.append(sock)
                    inputs.remove(sock)
                else:
                    if sock in output: #remove sock if it has written its response
                        output.remove(sock)
                    inputs.remove(sock)
                    sock.close()
                    del queues[sock]

        for sock in writeable_socks: #write data to sock
            try:
                ready_to_send = queues[sock].get_nowait()
            except queue.Empty:
                output.remove(sock)
            else:
                logger.add_to_log("sending data - " + str(ready_to_send))
                sock.send(ready_to_send)
                output.remove(sock)

        for sock in exception_socks: #catch exceptions
            inputs.remove(sock)
            if sock in output:
                output.remove(sock)
            sock.close()
            del queues[sock]


if __name__ == "__main__":
    main()
