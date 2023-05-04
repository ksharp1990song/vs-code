import socket
import struct

def handle_data(data):
    if isinstance(data, float):
        result = data * data
    elif isinstance(data, int):
        result = data * data
    else:
        result = None
    return result

def receive_data(sock):
    data = sock.recv(1024).decode('utf-8')
    try:
        int_value = int(data)
        result = handle_data(int_value)
    except ValueError:
        try:
            float_value = float(data)
            result = handle_data(float_value)
        except ValueError:
            result = None
            
    if result is not None:
        sock.sendall(str(result).encode('utf-8'))
        
def main():
    host = '127.0.0.1'
    port = 5000
    
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen(1)
        
        while True:
            conn, addr = s.accept()
            with conn:
                    receive_data(conn)
                    
if __name__ == '__main__':
    main()