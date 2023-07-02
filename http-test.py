import requests
import time

TCP_PORT = 8080
SWITCH_IP = '192.168.178.78'
HTTP_URL = "http://{}:{}/".format(SWITCH_IP, TCP_PORT)

def getRoute(url, name):
  print("\nRequest: {}".format(name))
  request = requests.get(HTTP_URL + url)
  print("Status Code: {}".format(request.status_code))
  if request.status_code == 200:
    print("HTML: {}".format(request.text))
  else:
    print("HTML: UNKNOWN")
  pass

def main():
  try:
    while True:
      time.sleep(2)
      #healthCheck = requests.get(HTTP_URL)
      getRoute("", "Health Check")
      time.sleep(2)
      #titleId = requests.get(HTTP_URL + "titleId")
      getRoute("titleId", "Title ID")
      time.sleep(2)
      #refreshMetadata = requests.get(HTTP_URL + "refreshMetadata")
      getRoute("refreshMetadata", "Metadata")
      
  except KeyboardInterrupt:
    print('\nExiting programm!')
    exit()

if __name__ == '__main__':
    main()