from RPi import GPIO
from RPLCD.gpio import CharLCD
#from RPLCD import CursorMode
class lcd :
	pre = ""
	c = 1
	def __init__(self) :
		self.dis  = CharLCD(numbering_mode=GPIO.BOARD,cols=16, rows=2, pin_rs=35, pin_e=18, pins_data=[16, 11, 12, 15])
		#self.dis.cursor_mode = CursorMode.blink
	def write(self,s) :
		if s !=  self.pre:
			self.dis.clear()
			self.dis.cursor_pos = (0,0)
			self.dis.write_string(s)
			self.pre = s
