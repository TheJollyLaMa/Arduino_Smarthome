#!/usr/bin/python
import sys
sys.path.insert(0, '/usr/lib/python2.7/bridge/')
import urllib2
import json
import fetch_and_log_functions as filefriend
from bridgeclient import BridgeClient as bridgeclient

ipinfo_url = "http://ipinfo.io"
res = urllib2.urlopen(ipinfo_url).read()
ip_data = json.loads(res)
location = ip_data['loc'].split(',')
latitude = location[0]
longitude = location[1]

"""
https://openweathermap.org/current for properties to response from weather_url
"""

weather_url = 'http://api.openweathermap.org/data/2.5/forecast?lat={}&lon={}&appid=&units=metric'.format(latitude, longitude)

r = urllib2.urlopen(weather_url).read()
weather_data = json.loads(r)
weatherForecastData = json.dumps(weather_data['list'])


total_days = bridgeclient().get('total_days')
year = bridgeclient().get('year')
path = '/mnt/sda1/arduino/www/jonesGreenHouse/data/' + str(year) + '/doy/' + str(total_days) + '/'
data_file = path + 'weatherForecastData.json'
filefriend.file_it(path, data_file, weatherForecastData)

"""
temp = weather_data['main']['temp']
wind_speed = weather_data['wind']['speed']
latitude = weather_data['coord']['lat']
longitude = weather_data['coord']['lon']
description = weather_data['weather'][0]['description']
print('Temperature : {} degree celcius'.format(temp))
print('Wind Speed : {} m/s'.format(wind_speed))
print('Latitude : {}'.format(latitude))
print('Longitude : {}'.format(longitude))
print('Description : {}'.format(description))
"""
