# coding:utf8 
# import sys
# print sys.path
# sys.path.append('/usr/local/lib/python2.7/site-packages')
# print sys.path

import SocketServer
import subprocess
import string
import time

class serverInfo(SocketServer.BaseRequestHandler):
    """docstring for serverInfo"""
    def recvFile(self,filename):
        print "Start receive file"
        f = open(filename, 'wb')
        self.request.send('ready')
        while True:
            data = self.request.recv(4096)
            if data == 'EOF':
                print "receive file success!"
                break
            f.write(data)
        f.close()

    def sendFile(self, filename):
        print "start send file"
        self.request.send('ready')
        time.sleep(1)
        f = open(filename, 'rb')
        while True:
            data = f.read(4096)
            if not data:
                break
            self.request.send(data)
        f.close()
        time.sleep(1)
        self.request.send('EOF')
        print "send file success!"

    def handle(self):
        print "get connection from:",self.client_address
        while True:
            try:
                data = self.request.recv(4096)
                print "get Data:", data
                if not data:
                    print "break the connection!"
                    break
                else:
                    action, filename = data.split()
                    if action == "put":
                        self.recvFile(filename)
                    elif action == "get":
                        self.sendFile(filename)
                    else:
                        print "Get Error!"
                        continue
            except Exception, e:
                print "Error: ",e

if __name__ == "__main__":
    host = ''
    port = 60000
    s = SocketServer.ThreadingTCPServer((host,port),serverInfo)
    s.serve_forever()

