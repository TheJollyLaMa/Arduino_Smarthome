#!/usr/bin/python
import logging
from pymodbus.client.sync import ModbusTcpClient
import time
import os
def logIt():
    FORMAT = ('%(asctime)-15s %(threadName)-15s '
              '%(levelname)-8s %(module)-15s:%(lineno)-8s %(message)s')
    logging.basicConfig(filename='modbus.log',format=FORMAT)
    log = logging.getLogger()
    log.setLevel(logging.DEBUG)

    UNIT = 0x1

def get_data():
    client = ModbusTcpClient('192.168.0.244', 502)
    client.connect()
    rr = client.read_holding_registers(0, 80, unit=1)
    client.close()
    return rr.registers

def handle_data():
    data = get_data()
    charge_state = ["START", "NIGHT_CHECK", "DISCONNECT", "NIGHT",
                    "FAULT", "MPPT", "ABSORPTION", "FLOAT", "EQUALIZE", "SLAVE"]

    alarms = ["RTS open", "RTS shorted", "RTS disconnected", "Heatsink temp sensor open",
              "Heatsink temp sensor shorted", "High temperature current limit", "Current limit",
              "Current offset", "Battery sense out of range", "Battery sense disconnected",
              "Uncalibrated", "RTS miswire", "High voltage disconnect", "Undefined",
              "system miswire", "MOSFET open", "P12 voltage off", "High input voltage current limit",
              "ADC input max", "Controller was reset", "Alarm 21", "Alarm 22", "Alarm 23", "Alarm 24"]

    faults = ["overcurrent", "FETs shorted", "software bug", "battery HVD", "array HVD",
              "settings switch changed", "custom settings edit", "RTS shorted", "RTS disconnected",
              "EEPROM retry limit", "Reserved", " Slave Control Timeout",
              "Fault 13", "Fault 14", "Fault 15", "Fault 16"]
    if data:
        update = time.ctime()
        volt_scaling_w = data[0]
        volt_scaling_f = data[1]
        volt_scaling = volt_scaling_w + (volt_scaling_f/0.00000000000000002)
        current_scaling_w = data[2]
        current_scaling_f = data[3]
        current_scaling = current_scaling_w + (current_scaling_f/0.00000000000000002)
        data_entry = {
           'update': update,
           'battery_terminal_voltage' : round(data[25] / volt_scaling, 2),
           'battery_sense_voltage' : round(data[26] / volt_scaling, 2),
           'array_voltage' : round(data[27] / volt_scaling, 2),
           'battery_current' : round(data[28] / current_scaling , 2),
           'array_current' : round(data[29] / current_scaling , 2),
           'heat_sink_temp_C' : data[35],
           'heat_sink_temp_F' : (data[35] * 9/5) + 32,
           'charging_current' : round(data[39] / current_scaling, 2),
           'charging_state' : charge_state[data[50]],
           'target_voltage' : data[51],
           'output_power' : data[58],
           'input_power' : data[59],
           'sweep_Pmax' : data[60],
           'sweep_Vmp' : data[61],
           'sweep_Voc' : data[62],
           'battery_temp_C' : data[36],
           'battery_temp_F' : (data[36] * 9/5) + 32,
           'total_kwh_resetable' : data[56],
           'total_kwh' : data[57],
           'alarm' : data[46],
           'fault' : data[44]

        }
    return data_entry

def is_file_empty(file_path):
    """ Check if file is empty by confirming if its size is 0 bytes"""
    # Check if file exist and it is empty
    return os.path.exists(file_path) and os.stat(file_path).st_size == 0
