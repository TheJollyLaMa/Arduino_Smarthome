#!/usr/bin/python
import os
import sys
sys.path.insert(0, '/usr/lib/python2.7/bridge/')
import json
import fetch_and_log_functions as filefriend
import fetchtristar
import fetchsmarthome
import solarmail_functions as SolarMailer
from bridgeclient import BridgeClient as bridgeclient

total_days = bridgeclient().get('total_days')
year = bridgeclient().get('year')
rtc_dt = bridgeclient().get('RTC_dt')
Power_Data = {}

fetchtristar.logIt()
Tristar_Data = fetchtristar.handle_data()
Tristar_Data['update'] = rtc_dt
Power_Data.update(Tristar_Data)
for each, value in Tristar_Data.items():
    print each + ': ' + str(value)
    bridgeclient().put(each,str(value))
fetchsmarthome.logIt()
Smarthome_Data = fetchsmarthome.handle_data()
Power_Data.update(Smarthome_Data)
for each, value in Smarthome_Data.items():
    print each + ': ' + str(value)
    bridgeclient().put(each,str(value))

path = '/mnt/sda1/arduino/www/jonesGreenHouse/data/' + str(year) + '/doy/' + str(total_days) + '/'
data_file = path + 'powerData.json'
print data_file
filefriend.file_it(path, data_file, Power_Data)
