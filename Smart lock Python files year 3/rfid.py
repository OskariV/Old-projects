from SimpleMFRC522 import SimpleMFRC522
class rfid:
	c = 1
	def __init__(self):
		self.reader = SimpleMFRC522()
		self.listOfAllowedIds = []
	def read(self):
		id, text =self.reader.read()
		return id,text
	def write(self,text):
		self.reader.write(text)
	def getAllowedTags(self):
		return self.listOfAllowedIds
	#@classmethod
	def addAllowedTag(self,idNumber):
		if idNumber not in self.listOfAllowedIds:
			self.listOfAllowedIds.append(idNumber)
			print("Allowed tags are")
			for x in self.listOfAllowedIds:
				print(x)
	def removeAllowedTag(self,idNumber):
		if idNumber in self.listOfAllowedIds:
			self.listOfAllowedIds.remove(idNumber)
			print("Allowed tags are")
			for x in self.listOfAllowedIds:
				print(x)
	def checkIfAllowed(self,tag):
		rValue  = False
		allowed = self.getAllowedTags()
		for x in allowed:
			if x ==  tag:
				rValue = True
		return rValue

	#@classmethod
	def addTextToTag(self):
		print("counter is " + str(self.c))
		identification = "allowed user number " + str(self.c)
		print(identification)
		self.write(identification)
		self.c += 1
	def clearTag(self):
		temp = rfid()
		self.write("")
