import socket
import threading


class Server(object):
    # create a sockets
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Store the connected clients
    connections = []

    def __init__(self):
        self.s.bind(("127.0.0.1",16012))
        self.s.listen(1)
        

    def handler(self, c, addr):  
        while True:
            data = c.recv(1024)
            for conn in self.connections:
                if conn != c:
                    conn.send(data)
            if not data:
                print("{}:{} Client disconnected".format(addr[0],addr[1]))
                self.connections.remove(c)
                c.close()
                break
            # print(data)

    def run(self):
        print("Server started ....")
        while True:
            c, addr = self.s.accept()
            cThread = threading.Thread(target=self.handler, args=(c, addr))
            cThread.daemon = True #kills the thread when the main process exits
            cThread.start()
            self.connections.append(c)
            print("{}:{} Connected".format(addr[0], addr[1]))


server = Server()
server.run()
