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

// this is the pin from the camera strobe trigger
#define CAMERA_CHANNEL_PIN 2
#define MOTOR_LEFT_PIN 4
#define MOTOR_RIGHT_PIN 5
#define BF_CHANNEL_PIN 7
#define GFP_CHANNEL_PIN 8
#define RFP_CHANNEL_PIN 9
#define IRFP_CHANNEL_PIN 10
#define MAX_TRIGGERS 4




// structure to store the details of an LED channel trigger
typedef struct {
  LEDEngine LED;
  uint8_t motor_position;
  bool active;
} trigger;

// global vars here
StepperMotorBSC201 stepper;
trigger trigger_BF;
trigger trigger_GFP;
trigger trigger_RFP;
trigger trigger_IRFP;
trigger triggers[MAX_TRIGGERS];


volatile byte state = LOW;
volatile byte motor = 0;

void setup() {
  // set up the serial port to receive data
  Serial.begin(115200);

  // set up the stepper motor 
  stepper = StepperMotorBSC201(MOTOR_LEFT_PIN, MOTOR_RIGHT_PIN);

  // set up the digital pins used to trigger LEDs and the camera
  trigger_BF   = {LEDEngine(  BF_CHANNEL_PIN), 0, false};
  trigger_GFP  = {LEDEngine( GFP_CHANNEL_PIN), 0, false};
  trigger_RFP  = {LEDEngine( RFP_CHANNEL_PIN), 0, false};
  trigger_IRFP = {LEDEngine(IRFP_CHANNEL_PIN), 1, false};

  // set up the input/strobe triggers
  // use the internal 20kOhm pull-up resistor NOTE: the logic is inverted due to the pullup
  pinMode(CAMERA_CHANNEL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CAMERA_CHANNEL_PIN), rising_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(CAMERA_CHANNEL_PIN), falling_interrupt, FALLING);
}



void rising_interrupt() {
  
}

void falling_interrupt() {
  
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
      case 'A':
        // ACQUIRE
        // TODO(arl): This is just a test trigger
        trigger_BF.LED.trigger(200);
        break;
        
      case 'S':
        // SETUP
        break;

      case 'I':
        // INFO

        // return the stepper motor position
        Serial.println(stepper.motor_position());
        break;
    }

  

  }
  
  return 0;
}



void loop() {
  // listen to the serial port
  listen_serial_port();

}
