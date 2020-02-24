
import recogniser
import gDrive
import sys
from time import sleep
from pin import pin
from rfid import rfid
from RPLCD.gpio import CharLCD
from RPi import GPIO
from time import sleep
from lcd import lcd
from ble_sub import finger


FINGER = finger()
FINGER.connect("F4:16:AF:5C:CA:6A", "00 00 00 00")
RFID = rfid()
BUZZER = pin(40,False)
BUZZER.write(False)
BUTTON = pin(37,True)
DBUTTON = pin(36,True)
CONFMODE = False
recog = recogniser.faceRecog()
drive = gDrive.gDrive('/home/pi/Documents')
LCD = lcd()
file = open("users.txt","r+")
users = file.read().split(",")
file.close()
try:
	while True:
		if BUTTON.read():
			while(BUTTON.read()):
				sleep(0.5)
			CONFMODE = not CONFMODE
		if CONFMODE is False:
			print("normal mode")
			LCD.write('Please show Tag\n\rOr fingerprint')
			FINGER.send_scan(False)
			finger = FINGER.send_find()
			if finger != None:
				if finger != 0:
					LCD.write('Authorized user\n\r'+users[finger-1])
					BUZZER.write(True)
					sleep(1)
					BUZZER.write(False)
				else:
					LCD.write('Unauthorized \n\ruser')
					sleep(2)
			id, text = RFID.read()
			if id is not None:
				print("ID: %s" % (id))
				LCD.write('tag noticed\n\rfetching data')
				if drive.download("ymls",str(id)+'.yml',"trainer"):
					LCD.write('Taking photo')
					print("user file found")
					if recog.detectUser(id):
						print("user found")
						BUZZER.write(True)
						LCD.write('Authorized user\n\rlock opens')
						sleep(1)
						BUZZER.write(False)
					else:
						drive.upload("unknown", "User."+str(id)+".jpg", "unknownUser")
						print("user not found")
						LCD.write('Unauthorized\n\ruser!')
						sleep(2)
				else:
					print("no user file found")
					LCD.write('This tag is not\n\rregistered')
					sleep(2)
		else:
			print("you have entered conf mode, please hold tag near the scanner")
			LCD.write('Configuration\n\radd tag')
			if DBUTTON.read():
				while DBUTTON.read():
					sleep(0.5)
				file = open("users.txt","w+")
				file.write(" ")
				file.close()
				users.clear()
				FINGER.send_del_all()
				drive.deleteAll("ymls")
				print("del all data")
				LCD.write('You deleted all data')
			id, text = RFID.read()
			if id is not None:
				LCD.write('Taking photo')
				recog.dataGather(id)
				drive.deleteIfIs("ymls",str(id)+".yml")
				drive.upload("ymls", recog.dataTrain(id), "trainer")
				RFID.addAllowedTag(id)
				LCD.write('Scan finger')
				file = open("users.txt","a+")
				num = len(users)
				if num <= 0:
					num = 1
				FINGER.send_scan(True)
				print("eee")
				bruh = FINGER.send_store(num)
				if bruh != None:
					file.write(str(num)+" "+str(id)+",")
					file.close()
					file = open("users.txt","r+")
					users = file.read().split(",")
					file.close()
				else:
					file.close()
				print("%s has been added"%(id))
				LCD.write('New user\n\radded')
				sleep(3)
				#else:
					#print("this person is allready authorized")
		sleep(0.5)
except KeyboardInterrupt:
	sleep(0.2)
	LCD.write('Program Exited\n\rSuccesfully')
	print('exiting')
	GPIO.cleanup()
