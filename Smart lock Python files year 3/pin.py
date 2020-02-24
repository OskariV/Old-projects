import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)

class pin :
	def __init__(self, pinNumber, input) :
		self.pinNumber = pinNumber
		if input == False :
			GPIO.setup(pinNumber, GPIO.OUT)
		else :
			GPIO.setup(pinNumber, GPIO.IN, pull_up_down=GPIO.PUD_UP)			#GPIO.setup(pinNumber, GPIO.IN)
	def __del__(self) :
		GPIO.cleanup()

	def write(self, state) :
		GPIO.output(self.pinNumber, state)

	def read(self) :
		return not GPIO.input(self.pinNumber)
