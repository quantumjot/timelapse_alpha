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

    Simple python interface to talks to an Arduino to synchronize timelapse
    acquisitions.  Takes care of scheduling the camera acquisitions, light
    source triggering and stepper motor based filter wheels using TTL pulses
    to synchronize. Talks to the host via a virtual COM port provided by the
    serial library.

    Params:


    Methods:
        add_trigger
        setup
        acquire
        info


    Serial message protocol:

        Running the multi-trigger acquisition:
        ACQ


        Requesting information from the Arduino:
        INF

        Setting up a trigger:
        SET,1,50,0

        Set channel 1 to 50 ms exposure with motor position 0. If we set up a
        trigger, it is automatically appended to the use list and fired in
        sequence as part of the acquisition protocol.

    Notes:


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

        trig = {'name':name,
                'duration_ms':duration_ms,
                'motor_pos':motor_pos,
                'ID': AVAILABLE_TRIGGERS.index(name)}
        self.triggers.append(trig)
        logger.info("Appended new trigger: {0:s}".format(name))

        # TODO(arl): write the trigger settings to the arduino


    def setup(self):
        """ Write the triggers to the Arduino using the serial protocol """
        if not self.triggers:
            logger.error("No triggers have been specified!")
            raise Exception


        for trigger in self.triggers:
            logger.info("Sending trigger {0:s} to Arduino...".format(trigger['name']))
            trig = "SET,{0:d},{1:d},{2:d}".format(trigger['ID'],
                                                  trigger['duration_ms'],
                                                  trigger['motor_pos'])
            self.arduino.write(trig)
            time.sleep(0.1)

        self.initialized = True


    def acquire(self):
        """ Run the acquisition settings """
        if not self.initialized:
            logger.error("Triggering has not been initialized.")
            raise Exception

        logger.info("Acquiring...")
        self.arduino.write("ACQ")

    def info(self):
        """ Get info from the arduino """
        # TODO(arl): request more informaton from the arduino, to sanity check
        # the settings
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
