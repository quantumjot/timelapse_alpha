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
import logging

AVAILABLE_TRIGGERS = ("BF", "GFP", "RFP", "IRFP")

# get the logger instance
logger = logging.getLogger('worker_process')

# if we don't have any handlers, set one up
if not logger.handlers:
    # configure stream handler
    formatter = logging.Formatter('[%(levelname)s][%(asctime)s] %(message)s',
        datefmt='%Y/%m/%d %I:%M:%S %p')
    console_handler = logging.StreamHandler()
    console_handler.setFormatter(formatter)

    logger.addHandler(console_handler)
    logger.setLevel(logging.DEBUG)


class ArduinoAcquisitionManager(object):
    """ ArduinoAcquisitionManager

    Talks to an Arduino to manager timelapse acquisitions
    """

    def __init__(self,
                 port="/dev/cu.usbmodem14601",
                 baud=115200,
                 timeout=1):

        self.arduino = serial.Serial(port, baud, timeout=timeout)
        time.sleep(1) # give the connection a second to settle

        self.triggers = []

        self.initialized = False


    def add_trigger(self, name, duration_ms=100, motor_pos=0):
        """ Add a trigger """

        if name not in AVAILABLE_TRIGGERS:
            raise ValueError("Trigger {0:s} not recognized.".format(name))

        if duration_ms < 0 or duration_ms>9999:
            raise ValueError("Duration of trigger is not in accepted range")

        trig = {'name':name, 'duration_ms':duration_ms, 'motor_pos':motor_pos}
        self.triggers.append(trig)
        logger.info("Appended new trigger: {0:s}".format(name))

        # TODO(arl): write the trigger settings to the arduino


    def setup(self):
        if not self.triggers:
            logger.error("No triggers have been specified!")
            raise Exception

        self.initialized = True


    def acquire(self):
        if not self.initialized:
            logger.error("Triggering has not been initialized.")
            raise Exception

        logger.info("Acquiring...")
        self.arduino.write("ACQ")

    def info(self):
        """ Get info from the arduino """
        self.arduino.write("INF")
        data = self.arduino.readline()
        if data:
            logger.info("Motor position: {0:s}".format(data.rstrip('\n')))




if __name__ == "__main__":
    aqm = ArduinoAcquisitionManager()
    aqm.add_trigger("BF", duration_ms=50, motor_pos=0)
    aqm.setup()
    while True:
        aqm.acquire()
        time.sleep(1)
        aqm.info()
