import sys
sys.path.insert(0, '/usr/lib/python2.7/bridge/')
import fetch_and_log_functions as filefriend
from bridgeclient import BridgeClient as bridgeclient

total_days = bridgeclient().get('total_days')
year = bridgeclient().get('year')
event_str = bridgeclient().get('Event_Log_String')
event = {
    "update": event_str
}

path = '/mnt/sda1/arduino/www/jonesGreenHouse/data/logs/' + str(year) + '/' + str(total_days) + '/'
data_file = path + 'event_log.json'
filefriend.file_it(path, data_file, event)
