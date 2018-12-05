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

#define CMD_SETUP 0 
#define CMD_ACQUIRE 1 
#define CMD_INFO 2


// structure to store the details of an LED channel trigger
typedef struct {
  LEDEngine LED;
  uint8_t motor_position;
  unsigned int duration_ms;
} trigger;


// global vars here
StepperMotorBSC201 stepper;
trigger trigger_BF;
trigger trigger_GFP;
trigger trigger_RFP;
trigger trigger_IRFP;


void setup() {
  // set up the serial port to receive data
  Serial.begin(115200);

  // set up the stepper motor 
  stepper = StepperMotorBSC201(MOTOR_LEFT_PIN, MOTOR_RIGHT_PIN);

  // set up the digital pins used to trigger LEDs and the camera
  trigger_BF   = {LEDEngine(  BF_CHANNEL_PIN, CAMERA_CHANNEL_PIN), 0, 50};
  trigger_GFP  = {LEDEngine( GFP_CHANNEL_PIN, CAMERA_CHANNEL_PIN), 0, 100};
  trigger_RFP  = {LEDEngine( RFP_CHANNEL_PIN, CAMERA_CHANNEL_PIN), 0, 100};
  trigger_IRFP = {LEDEngine(IRFP_CHANNEL_PIN, CAMERA_CHANNEL_PIN), 1, 100};
  
}








/*
 listen to the serial port for commands
 all messages start with a header

 SET - initialize the triggers
 ACQ - acquire
 INF - return status info
*/
uint8_t listen_serial_port(void) {

  // read the data from the serial port, if available
  if(Serial.available() > 0) {

    // read the buffer as a string
    String data = Serial.readString();

    // get the header
    char header = data.charAt(0);

    switch (header) {
      case 'S':
        break;
      case 'A':
        // TODO(arl): This is just a test trigger
        trigger_BF.LED.trigger(200);
        break;
      case 'I':
        Serial.println(stepper.motor_position());
        break;
    }

  

  }
  
  return 0;
}



void loop() {
  // listen to the serial port
  listen_serial_port();

  // trig_BF.LED.trigger(100);
  delay(1);

}
