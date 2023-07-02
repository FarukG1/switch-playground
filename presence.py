import socket
import struct
import json

TCP_PORT = 0xCAFE
SWITCH_IP = '192.168.178.78'
PACKETMAGIC = 0xFFAADD23

#Defines a title packet
class Title:

    def __init__(self, raw_data):
        unpacker = struct.Struct('2L612s')
        enc_data = unpacker.unpack(raw_data)
        self.magic = int(enc_data[0])
        if int(enc_data[1]) == 0:
            self.pid = int(enc_data[1])
            self.name = 'Home Menu'
        else:
            self.pid = int(enc_data[1])
            self.name = enc_data[2].decode('utf-8', 'ignore').split('\x00')[0]

def main():
    switch_server_address = (SWITCH_IP, TCP_PORT)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(10)
    try:
      sock.connect(switch_server_address)
      data = sock.recv(628)
      title = Title(data)
      sensorJson = {
          "state": "on",
          "id": str(hex(title.pid)).replace("x",""),
          "title": title.name
      }
      sensorString = json.dumps(sensorJson)
      print(sensorString)
    except:
      sensorJson = {
          "state": "off",
          "id": -1,
          "title": "unknown"
      }
      sensorString = json.dumps(sensorJson)
      print(sensorString)
    exit()

if __name__ == '__main__':
    main()