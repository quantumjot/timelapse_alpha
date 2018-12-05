#!/usr/bin/env python
#-------------------------------------------------------------------------------
# Name:     timelapse_alpha
# Purpose:  Arduino code to talk to a python interface. Takes care of scheduling
#           the camera acquisitions, light source triggering and stepper motor
#           based filter wheels using TTL pulses to synchronize. Talks to the
#           host via a virtual COM port
#
# Authors:  Alan R. Lowe (arl) a.lowe@ucl.ac.uk
#
# License:  See LICENSE.md
#
# Created:  05/12/2018
#-------------------------------------------------------------------------------

__author__ = "Alan R. Lowe"
__email__ = "a.lowe@ucl.ac.uk"

import time
import serial


class ArduinoAcquisitionManager(object):
    """ ArduinoAcquisitionManager

    Talks to an Arduino to manager timelapse acquisitions
    """

    def __init__(self,
                 com_port="COM1",
                 baud=115200,
                 timeout=0.1):

        self.arduino = serial.Serial(com_port, baud, timeout=timeout)
        time.sleep(10) # give the connection a second to settle

        self.triggers = []


    def add_trigger(self, name="None", duration_ms=100, motor_pos=0):
        """ Add a trigger """
        pass

    def acquire(self):
        pass

    def info(self):
        """ Get info from the arduino ""
        arduino.write("INF")

    def setup(self):
        pass


if __name__ == "__main__":
    pass
