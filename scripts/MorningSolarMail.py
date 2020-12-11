#!/usr/bin/env python
import sys
sys.path.insert(0, '/usr/lib/python2.7/bridge/')
import fetchtristar
from bridgeclient import BridgeClient as bridgeclient
import solarmail_functions as SolarMailer

total_days = bridgeclient().get('total_days')
Tristar_Data = fetchtristar.handle_data()
total_kWh = Tristar_Data['total_kwh']
SolarMailer.send_morning_email(total_kWh, total_days)
