__author__ = 'joser'
import requests
import json


# config
HOST = "52.17.44.249"
PORT = "1026"

# entities details
ATTRIBUTE_NAME = "h"
DEVICE_NAME = "pi"
FIWARE_SERVICE = "demotefcon"
THRESHOLD = 30

#main
def main():

    #query attributes
    headers = {'Fiware-Service': FIWARE_SERVICE, 'Content-Type': 'application/json', 'Accept': 'application/json'}
    url = "http://"+HOST+":"+PORT+"/v1/contextEntities/device:"+DEVICE_NAME+"/attributes/"+ATTRIBUTE_NAME

    #send request
    print "sending request to: " + url
    response = requests.get(url=url, headers=headers)

    #parse response
    response_json = json.loads(response.text)

    #print "Received value for " + ATTRIBUTE_NAME + " : " + response_json['attributes'][0]['value']

    #print results
    if(float(response_json['attributes'][0]['value'])>THRESHOLD):
        print("1")
    else:
        print("0")



if __name__ == '__main__':
    main()