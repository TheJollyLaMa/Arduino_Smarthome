import subprocess
import os
import re

cmd = ['iw', 'dev', 'wlan0-1', 'station', 'dump']
data = subprocess.Popen(cmd, stdout=subprocess.PIPE)
output = str(data.communicate())
output=output.split("signal: ")
output=output[1].split("-")
tmp = output[1].split(" [")
WiFiStatus = tmp[0]

print WiFiStatus


"""
output=output.split("signal: ")
output= output[1].rstrip('\t')
tmp = output.split(" [")
WiFiStatus = tmp[0]
"""
