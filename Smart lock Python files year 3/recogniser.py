import cv2
import numpy as np
from PIL import Image
import os

class faceRecog:
    def __init__(self):
        self.datafolder = "dataset"
        self.trainerfolder = "trainer/"
        self.cam = cv2.VideoCapture(0)
        self.cam.set(3, 640) # set video width
        self.cam.set(4, 480) # set video height
        self.face_detector = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

    def dataGather(self, userId): # Takes 30 jpegs as data under entered id
        face_id = userId
        print("\n [INFO] Initializing face capture. Look the camera and wait ...")

        # Empty the datafolder for new data
        path = self.datafolder
        imagePaths = [os.path.join(path,f) for f in os.listdir(path)]
        for f in imagePaths:
            os.remove(f)

        # Initialize individual sampling face count
        count = 0
        fucc = True
        while(fucc):
            ret, img = self.cam.read()
            img = cv2.flip(img, -1) # Flip camera vertically
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            faces = self.face_detector.detectMultiScale(gray, 1.3, 5)
            for (x,y,w,h) in faces:
                cv2.rectangle(img, (x,y), (x+w,y+h), (255,0,0), 2)
                count += 1
                # Save the captured image into the datasets folder
                cv2.imwrite(self.datafolder + "/User." + str(face_id) + '.' + str(count) + ".jpg", gray[y:y+h,x:x+w])
                cv2.imshow('image', img)
                k = cv2.waitKey(100) & 0xff # Press 'ESC' for exiting video
                if k == 27:
                    fucc = False
                elif count >= 30: # Take 30 face sample and stop video
                    fucc = False
            # Do a bit of cleanup
        print("\n [INFO] Exiting data gathering")

    def dataTrain(self, userId): #Uses data to compose a yml with Id, removes old data, return name of file
        path = self.datafolder
        recognizer = cv2.face.createLBPHFaceRecognizer()
        imagePaths = [os.path.join(path,f) for f in os.listdir(path)]
        faceSamples=[]
        ids = []
        for imagePath in imagePaths:
            PIL_img = Image.open(imagePath).convert('L') # convert it to grayscale
            img_numpy = np.array(PIL_img,'uint8')
            id = int(os.path.split(imagePath)[-1].split(".")[1])
            faces = self.face_detector.detectMultiScale(img_numpy)
            for (x,y,w,h) in faces:
                faceSamples.append(img_numpy[y:y+h,x:x+w])
                ids.append(id)
        recognizer.train(faceSamples, np.array(ids))
        # Save the model into trainer/trainer.yml
        recognizer.save(self.trainerfolder + str(userId) + '.yml') 
        # delete old images
        for f in imagePaths:
            os.remove(f)
        print("\n [INFO] faces trained, old images deleted Exiting Program")
        return (str(userId) + '.yml')

    def detectUser(self, userId): #Uses yml with id to recog user
        recognizer = cv2.face.createLBPHFaceRecognizer()
        recognizer.load(self.trainerfolder + str(userId) + '.yml')
        faceCascade = self.face_detector
        font = cv2.FONT_HERSHEY_SIMPLEX
        #iniciate id counter
        id = 0
        # names related to ids, only recognized should ever appear
        names = ['None', 'recognized', 'recog2', 'recog3', 'recog4', '']

        # Define min window size to be recognized as a face
        minW = 0.1*self.cam.get(3)
        minH = 0.1*self.cam.get(4)
        fucc = True
        count = 0
        exit = 0
        timeout = 0
        while (fucc):
            ret, img = self.cam.read()
            img = cv2.flip(img, -1) # Flip camera vertically
            gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
            faces = faceCascade.detectMultiScale(
                gray,
                scaleFactor = 1.2,
                minNeighbors = 5,
                minSize = (int(minW), int(minH)),
                )
            for(x,y,w,h) in faces:
                cv2.rectangle(img, (x,y), (x+w,y+h), (0,255,0), 2)
                id, confidence = recognizer.predict(gray[y:y+h,x:x+w])
                # Check if confidence is less them 100 ==> "0" is perfect match 
                if (confidence < 60):
                    id = names[1]
                    confidence = "  {0}%".format(round(100 - confidence))
                    count += 1
                    exit = 0
                    timeot = 0
                    if (count > 10):
                        print("\n [INFO] Exiting Program found user")
                        return True
                else:
                    id = "unknown"
                    confidence = "  {0}%".format(round(100 - confidence))
                    count = 0
                    exit += 1
                    timeout = 0
                    if (exit > 30):
                        cv2.imwrite("unknownUser" + "/User." + str(userId) + ".jpg", gray[y:y+h,x:x+w])
                        print("\n [INFO] Exiting Program no matching face found")
                        return False
                cv2.putText(img, str(id), (x+5,y-5), font, 1, (255,255,255), 2)
                cv2.putText(img, str(confidence), (x+5,y+h-5), font, 1, (255,255,0), 1)
            cv2.imshow('camera',img)
            timeout += 1
            if (timeout > 120):
                print("\n [INFO] Exiting Program Timeout")
                return False
            k = cv2.waitKey(10) & 0xff # Press 'ESC' for exiting video
            if k == 27:
                fucc = False
		# Do a bit of cleanup
        print("\n [INFO] Exiting Program and cleanup stuff")

    def __del__(self):
        self.cam.release()
        cv2.destroyAllWindows()
