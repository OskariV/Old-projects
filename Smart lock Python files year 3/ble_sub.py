from threading import Thread
from queue import Queue
import bluepy
from bluepy.btle import Peripheral, DefaultDelegate, BTLEException
import numpy as np
import time

class BluepyExample(DefaultDelegate):

    def __init__(self, address, passcode, type=bluepy.btle.ADDR_TYPE_PUBLIC):
        super().__init__()
        self.passcode = passcode
        self.connected = False
        self._peripheral_address = address
        self._peripheral_address_type = type
        self._peripheral = None
        self._tx_queue = Queue()
        self._rx_queue = Queue()
        self.passw = False
        self.dele = False
        self.delAll = False
        self.stored = False
        self.wait_data = False
        self.have_finger = False
        self.finger_fail = False
        self.scaning = False

        self.place = 0
        self.connect()

    def connect(self):
        self._bluepy_thread = Thread(target=self._bluepy_handler)
        self._bluepy_thread.name = "bluepy_handler"
        self._bluepy_thread.daemon = True
        self._bluepy_thread.start()
        self.connected = True

    def handleNotification(self, cHandle, data):

        s = " ".join(["{:02x}".format(x) for x in data])
        print("Received Notification: " + str(data))
        if str(data)[2:5] == "Got":
            self.have_finger = True
            self.scaning = False
            print("Got fingerprint")
        elif str(data)[2:7] == "Found":
            self.place = int(str(data).split(" ")[1])
        elif str(data)[2:7] == "Store":
            self.store = True
        elif str(data)[2:7] == "Right":
            self.passw = True
        elif str(data)[2:13] == "Deleted ALL":
            self.delAll = True
        elif str(data)[2:9] == "Deleted":
            self.dele = True
        elif str(data)[2:6] == "FAIL":
            self.finger_fail = True
            self.scaning = False

        self.wait_data = False

    def _bluepy_handler(self):
        try:
            # Connect to the peripheral
            self._peripheral = Peripheral(self._peripheral_address, self._peripheral_address_type)
            # Set the notification delegate
            self._peripheral.setDelegate(self)
            setup_data = b"\x01\x00"
            notify = self._peripheral.getCharacteristics(uuid='6e400003-b5a3-f393-e0a9-e50e24dcca9e')[0]
            notify_handle = notify.getHandle() + 1
            self._peripheral.writeCharacteristic(notify_handle, setup_data, withResponse=True)
            print("Connected")
            c = self._peripheral.getCharacteristics(uuid='6e400002-b5a3-f393-e0a9-e50e24dcca9e')[0]
            print("Checking password...")
            # Send and check password
            self.wait_data = True
            m = "pass " + self.passcode
            c.write(bytes("pass " + self.passcode, encoding="utf-8"))
            while self.get_wait():
                self._peripheral.waitForNotifications(0.1)
            if not self.get_pass():
                print("Wrong password")
                exit(1)
            print("Correct password")
            while True:
                while not self._tx_queue.empty():
                    msg = self._tx_queue.get_nowait()
                    msg_bytes = bytes(msg, encoding="utf-8")
                    c.write(msg_bytes)
                    print("sended")
                self._peripheral.waitForNotifications(0.1)

        except BTLEException as e:
            print(e)
            self.connected = False
            self.reset_all()
            self.connect()

    def reset_all(self):
        self.passw = False
        self.stored = False
        self.dele = False
        self.wait_data = False
        self.have_finger = False
        self.delAll = False
        self.finger_fail = False
        self.scaning = False
        self.place = 0

    def check_con(self):
        if not self.connected:
            self.connect()

    def get_store(self):
        t = self.store
        self.store = False
        return t

    def get_del(self):
        t = self.dele
        self.dele = False
        return t

    def get_del_all(self):
        t = self.delAll
        self.delAll = False
        self.reset_all()
        return t

    def get_find(self):
        t = self.place
        self.place = 0
        return t

    def get_scaning(self):
        return self.scaning

    def set_scaning(self, input):
        self.scaning = input

    def get_wait(self):
        return self.wait_data

    def get_pass(self):
        t = self.passw
        self.passw = False
        return t

    def get_f_fail(self):
        t = self.finger_fail
        self.finger_fail = False
        return t

    def check_got(self):
        if self.have_finger:
            self.have_finger = False
            return True
        return False

    def get_got(self):
        return self.have_finger

    def send(self, message, wait):
        self._tx_queue.put_nowait(message)
        self.wait_data = wait

    def get_f_print(self):
        s = self.fingers.get_nowait()
        return s

    def reform_data(self):
         x = []
         verf = 0
         count = 0
         while not self._rx_queue.empty():
             count += 1
             s = self._rx_queue.get_nowait()
             c = s.hex()
             if count == 13:
                 verf = c[-8:]
                 c = c[:-8]
             msg = [int(c[i:i+2], 16) for i in range(0,len(c),2)]
             x.extend(msg)
         sum = 0
         for i in x:
             sum += i
         if sum == int(verf, 16):
             print("Sum match")
             return x
         else:
             return []

class finger():
    def __init__(self):

        self.f = 0

    def connect(self, address, passcode):
        addr_type = bluepy.btle.ADDR_TYPE_RANDOM
        self.f = BluepyExample(address, passcode, addr_type)

    def password(self, line , n):
        line = str(line)
        return " ".join(["{:02d}".format(int(x)) for x in [line[i:i+n] for i in range(0, len(line), n)]])

    def send_stop(self):
        self.f.send("stop ", True)
        while self.f.get_wait():
                time.sleep(0.1)

    def send_scan(self, input):
        if (not self.f.get_scaning() and not self.f.get_got()) or input:
        #if not self.f.check_got() :
            while not self.f.get_got():
                print("send scan")
                self.f.set_scaning(True)
                self.f.send("fingerprint ", False)
                if not input:
                    return True
                while self.f.get_scaning() and input:
                    time.sleep(0.1)
            print("Now have finger")
            return True
        return False

    def send_store(self, place):
        if self.f.check_got():
            print("send Store")
            self.f.send("store " + str(place), True)
            while self.f.get_wait():
                time.sleep(0.1)
            return self.f.get_store()

    def send_find(self):
        if self.f.check_got():
            print("send find")
            self.f.send("find ", True)
            while self.f.get_wait():
                time.sleep(0.1)
            return self.f.get_find()

    def send_del(self, place):
        self.f.send("delete " + str(place), True)
        while self.f.get_wait():
            time.sleep(0.1)
        return self.f.get_del()

    def send_del_all(self):
        self.send_stop()
        self.f.send("delall ", True)
        while self.f.get_wait():
            time.sleep(0.1)
        return self.f.get_del_all()

if __name__ == "__main__":
    fin = finger()
    fin.connect("F4:16:AF:5C:CA:6A", "00 00 00 00")
    while True:
        msg = input()
        m = msg.split(" ")
        if msg == "q":
            break
        elif msg == "fi":
            fin.send_scan(False)
        elif msg == "fe":
            fin.send_scan(True)
        elif m[0] == "s":
            print(fin.send_store(int(m[1])))
        elif msg == "f":
            p = fin.send_find()
            if p == 0:
                print("No match")
            else:
                print("match in slot: " + str(p))
        elif m[0] == "d":
            print(fin.send_del(int(m[1])))
        elif msg == "da":
            print(str(fin.send_del_all()) + " Del all" )
