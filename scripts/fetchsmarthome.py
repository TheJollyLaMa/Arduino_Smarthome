#!/usr/bin/python
import time
import logging
import urllib2
import json

def logIt():
    FORMAT = ('%(asctime)-15s %(threadName)-15s '
              '%(levelname)-8s %(module)-15s:%(lineno)-8s %(message)s')
    logging.basicConfig(filename='smarthome_data.log',format=FORMAT)
    log = logging.getLogger()
    log.setLevel(logging.DEBUG)

    UNIT = 0x1

def get_data():
    smarthome_data = urllib2.urlopen("http://192.168.0.208/data/get").read()
    smarthome_data = json.loads(smarthome_data)
    return smarthome_data

def handle_data():
    data = get_data()
    if data:
        data_entry = {
            'rp1' : data['value']['realPower1'],
            'rp2' : data['value']['realPower2'],
            'rp3' : data['value']['realPower3'],
            'rp4' : data['value']['realPower4']
        }
    return data_entry
