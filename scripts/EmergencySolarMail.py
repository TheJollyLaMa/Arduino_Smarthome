#!/usr/bin/env python
import sys
sys.path.insert(0, '/usr/lib/python2.7/bridge/')
import fetchtristar
from bridgeclient import BridgeClient as bridgeclient
import solarmail_functions as SolarMailer

Greenhouse_Data = bridgeclient().getall()
Tristar_Data = fetchtristar.handle_data()

SolarMailer.send_emergency_notification_email(Greenhouse_Data, Tristar_Data)
