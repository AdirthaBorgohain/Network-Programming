#import socket
from socket import socket, AF_INET, SOCK_STREAM
import sys
import datetime


class Fruit:
    qty = 0

    def __init__(self, qty, t_stamp="--", unique_clients=0):
        self.qty = qty
        self.t_stamp = t_stamp
        self.unique_clients = unique_clients

    def reduceQty(self, qty):
        self.qty = self.qty - qty


def parseRequest(msg):
    request = msg.split(" ")
    try:
        return request[0], int(request[1])
    except:
        print("Plese send proper request")
        sys.exit(1)


def printFruits():
    print("\nAvailable items")
    print("Product\t\tQty_left\tLast Sold")
    print("Mango\t\t{}\t\t{}".format(mango.qty, mango.t_stamp))
    print("Orange\t\t{}\t\t{}".format(orange.qty, orange.t_stamp))
    print("Guava\t\t{}\t\t{}".format(guava.qty, guava.t_stamp))
    print("Banana\t\t{}\t\t{}".format(banana.qty, banana.t_stamp))
    print("-----------------------------------------------------")

def doTransaction(reqFruit, qty):
    if(reqFruit == "M"):
        # Mango
        if(qty <= mango.qty):
            ret_message = "Transaction successful"
            mango.reduceQty(qty)
            mango.t_stamp = datetime.datetime.now()
        else:
            ret_message = "Transaction error"
    elif(reqFruit == "O"):
        # Orange
        if(qty <= orange.qty):
            ret_message = "Transaction successful"
            orange.reduceQty(qty)
            orange.t_stamp = datetime.datetime.now()
        else:
            ret_message = "Transaction error"
    elif(reqFruit == "G"):
        # Guava
        if(qty <= guava.qty):
            ret_message = "Transaction successful"
            guava.reduceQty(qty)
            orange.t_stamp = datetime.datetime.now()
    elif(reqFruit == "B"):
        # Banana
        if(qty <= banana.qty):
            ret_message = "Transaction successful"
            banana.reduceQty(qty)
            banana.t_stamp = datetime.datetime.now()
        else:
            ret_message = "Transaction error"
    else:
        ret_message = "Transaction error"
    return ret_message
def addIpToList(addr):
    clients.append(addr)
    print("Unique clients: ")
    print(clients)

clients = []
mango = Fruit(10)
orange = Fruit(15)
guava = Fruit(20)
banana = Fruit(30)
if __name__ == "__main__":
    host = "127.0.0.1"
    port = 16012
    s = socket(AF_INET, SOCK_STREAM, 0)
    s.bind((host, port))
    s.listen(5)
    printFruits()
    print("Waiting for orders from client..")
    while(True):
        q, addr = s.accept()
        # get the transaction request from the client
        msg = q.recv(100)
        print("Order from client: '{}'".format(msg.decode()))
        # Parse the fruit name and qty
        reqFruit, qty = parseRequest(msg.decode())

        # Check if qty exist if yes then do transaction
        result = doTransaction(reqFruit, qty)
        print(result)
        if( result == "Transaction successful"):
            addIpToList(addr)
        printFruits()
        q.send("Transaction is successfully done".encode())
s.close()
