/*
 * timelapse_alpha
 * 
 * Arduino code to talk to a python interface. Takes care of scheduling the camera
 * acquisitions, light source triggering and stepper motor based filter wheels using 
 * TTL pulses to synchronize.
 * 
 * Talks to the host via a virtual COM port
 * 
 * Alan R. Lowe (a.lowe@ucl.ac.uk)
 * December 2018
 * lowe.cs.ucl.ac.uk
 * 
 */

#include "ledengine.h"
#include "bsc201.h"


#define BF_CHANNEL_PIN 7
#define GFP_CHANNEL_PIN 8
#define RFP_CHANNEL_PIN 9
#define IRFP_CHANNEL_PIN 10
#define CAMERA_CHANNEL_PIN 13

#define MOTOR_LEFT_PIN 11
#define MOTOR_RIGHT_PIN 12


// structure to store the details of an LED channel trigger
typedef struct {
  LEDEngine LED;
  uint8_t motor_position;
  unsigned int duration_ms;
} trigger;



void setup() {
  // set up the serial port to receive data
  Serial.begin(115200);

  // set up the stepper motor 
  StepperMotorBSC201 stepper(MOTOR_LEFT_PIN, MOTOR_RIGHT_PIN);

  // set up the digital pins used to trigger LEDs and the camera
  trigger trig_BF   = {LEDEngine(  BF_CHANNEL_PIN, CAMERA_CHANNEL_PIN), 0, 50};
  trigger trig_GFP  = {LEDEngine( GFP_CHANNEL_PIN, CAMERA_CHANNEL_PIN), 0, 100};
  trigger trig_RFP  = {LEDEngine( RFP_CHANNEL_PIN, CAMERA_CHANNEL_PIN), 0, 100};
  trigger trig_IRFP = {LEDEngine(IRFP_CHANNEL_PIN, CAMERA_CHANNEL_PIN), 1, 100};
  
}









// listen to the serial port for commands
byte listen_serial_port(void) {
  return 0;
}



void loop() {
  // listen to the serial port
  listen_serial_port();

}
