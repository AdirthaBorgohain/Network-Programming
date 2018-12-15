#import socket
from socket import socket, AF_INET, SOCK_STREAM

if __name__ == "__main__":
    cont = 'y'
    while(cont == 'y' or cont == 'Y'):
        host = "127.0.0.1"
        port = 16012
        s = socket(AF_INET, SOCK_STREAM, 0)
        s.connect((host, port))
        print("Welcome to our shop\n")
        msg = input("Please enter the first letter of the item you want to buy followed by the quantity\n")
        s.send(msg.encode())
        msg = s.recv(100)
        print("Message from server: {}".format(msg.decode()))

        cont = input("Do you wish to perform more transactions? (Y/N) : ")
s.close()
