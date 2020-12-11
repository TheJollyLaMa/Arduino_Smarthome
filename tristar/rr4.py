#!/usr/bin/python
import os
import sys

sys.path.insert(0, '/usr/lib/python2.7/bridge/')
import json
import rrf
from bridgeclient import BridgeClient as bridgeclient

rrf.logIt()

total_days = bridgeclient().get('total_days')
year = bridgeclient().get('year')

Register_Update = rrf.handle_data()
for each, value in Register_Update.items():
    print each + ': ' + str(value)
    #remove bridge.put and just catch, print, and parse from the process on arduino java side
    bridgeclient().put(each,str(value))

path = '/mnt/sda1/arduino/www/jonesGreenHouse/data/' + str(year) + '/doy/' + str(total_days) + '/'
data_file = path + 'powerGenerationData.json'

print total_days
print year
print path

if os.path.exists(data_file):
    if rrf.is_file_empty(data_file):
        with open(data_file, 'w') as f:
            f.write("[")
            json.dump(Register_Update, f)
            f.write("]")
    else:
        with open(data_file, 'a+') as f:
            # Move read cursor to the start of file.
            f.seek(0)
            # If file is not empty then append '\n'
            data = f.read()
            if len(data) > 0 :
                f.seek(-1, os.SEEK_END)
                f.truncate()
                f.write(",\n")
            # Append text at the end of file
            json.dump(Register_Update, f)
            f.write("]")
else:
    with open(data_file, 'w') as f:
        f.write("[")
        json.dump(Register_Update, f)
        f.write("]")
