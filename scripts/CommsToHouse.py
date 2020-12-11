import subprocess
import os

cmd = ['ip', 'link', 'show', 'wlan0-1']
data = subprocess.Popen(cmd, stdout=subprocess.PIPE)
output = str(data.communicate())

output=output.split("state ")
tmp = output[1].split(" mode")
WiFiStatus = tmp[0]
print WiFiStatus
