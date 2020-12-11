import subprocess
import os

data_dir = '/mnt/sda1/arduino/www/jonesGreenHouse/Data'
destination = '192.168.0.33:/Users/J/Desktop'
cmd = ['scp', data_dir, destination]
p = subprocess.call(cmd)
