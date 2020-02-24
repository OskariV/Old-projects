import io
import os
import httplib2
from oauth2client import file, client, tools
from googleapiclient.discovery import build
from googleapiclient.http import MediaIoBaseDownload, MediaFileUpload

class gDrive:
	def __init__(self, secretsFolderPath):
		self.files = self.getDrive(secretsFolderPath)


	def getDrive(self, secretsFolderPath):
		SCOPES = 'https://www.googleapis.com/auth/drive'

		store = file.Storage(secretsFolderPath + '/credits-drive.txt')
		creds = store.get()
		if not creds or creds.invalid:
			args = tools.argparser.parse_args()
			args.noauth_local_webserver = True
			flow = client.flow_from_clientsecrets(secretsFolderPath + '/api_key.json', SCOPES)
			creds = tools.run_flow(flow, store, args)

		http = creds.authorize(httplib2.Http(timeout=None))
		service = build('drive', 'v3', http=creds.authorize(http))
		return service.files()

	def upload(self, folderName, filename, fromFolder):
		quar = "name = '" + folderName + "' and mimeType = 'application/vnd.google-apps.folder'"
		print(quar)
		#print("name = 'KaappiKuva' and mimeType = 'application/vnd.google-apps.folder'")
		resp = self.files.list(q=quar,fields="files(id,name)",pageSize=500).execute()
		folder_id = resp.get('files')[0].get('id')


		#Upload new picture
		file_metadata = {'name': filename,'parents':[folder_id]}
		media = MediaFileUpload(fromFolder + '/' + filename ,mimetype='text/yml', resumable=True)
		file = self.files.create(body=file_metadata, media_body=media, fields='id').execute()
		print("Kuva upattu")

	def download(self, folderName, filename, savePath):
		quar =  "name = '" + folderName + "' and mimeType = 'application/vnd.google-apps.folder'"
		resp = self.files.list(q=quar,fields="files(id,name)",pageSize=500).execute()
		folder_id = resp.get('files')[0].get('id')

		#Find file
		quar = "'" + folder_id + "' in parents"
		resp = self.files.list(q=quar,fields="files(id,name)",pageSize=500).execute()
		for file in resp.get('files', []):
			print("File id: " + file.get('id'))
			if file.get('name') == filename:
				req = self.files.get_media(fileId=file.get('id'))
				fh = io.BytesIO()
				downloader = MediaIoBaseDownload(fh, req)
				done = False
				while done is False:
					status, done = downloader.next_chunk()
					if status:
						print("Download %d%%." % int(status.progress() * 100))

				if not(os.path.exists(savePath)):
					os.makedirs(savePath)

				with io.open(savePath + "/"  + file.get('name'),'wb') as f:
					fh.seek(0)
					f.write(fh.read())
				f.close()
				return True
		return False

	def deleteAll(self, folderName):
		quar =  "name = '" + folderName + "' and mimeType = 'application/vnd.google-apps.folder'"
		resp = self.files.list(q=quar,fields="files(id,name)",pageSize=500).execute()
		folder_id = resp.get('files')[0].get('id')

		quar = "'" + folder_id + "' in parents"
		resp = self.files.list(q=quar,fields="files(id,name,createdTime)",pageSize=500).execute()
		for file in resp.get('files', []):
			self.files.delete(fileId=file.get('id')).execute()
			print("Deleted: " + file.get('id'))

	def deleteIfIs(self, folderName, fileName):
		quar =  "name = '" + folderName + "' and mimeType = 'application/vnd.google-apps.folder'"
		resp = self.files.list(q=quar,fields="files(id,name)",pageSize=500).execute()
		folder_id = resp.get('files')[0].get('id')

		quar = "'" + folder_id + "' in parents"
		resp = self.files.list(q=quar,fields="files(id,name,createdTime)",pageSize=500).execute()
		for file in resp.get('files', []):
			if file.get('name') == fileName:
				self.files.delete(fileId=file.get('id')).execute()
				print("Deleted File: ")
				print("User DELETED")
