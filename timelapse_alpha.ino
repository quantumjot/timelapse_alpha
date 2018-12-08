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

#include "sequencer.h"

// this is the pin from the camera strobe trigger
#define CAMERA_CHANNEL_PIN 2
#define MOTOR_LEFT_PIN 4
#define MOTOR_RIGHT_PIN 5
#define BF_CHANNEL_PIN 7
#define GFP_CHANNEL_PIN 8
#define RFP_CHANNEL_PIN 9
#define IRFP_CHANNEL_PIN 10
#define MAX_TRIGGERS 4

// states for trigger interrupts
volatile byte state = LOW;
volatile byte motor = HIGH;

void setup() {
  // set up the serial port to receive data
  Serial.begin(115200);

  // set up the trigger sequencer
  TriggerSequencer sequencer;
  sequencer.clear_triggers();
  sequencer.add_trigger(BF_CHANNEL_PIN, 0, true);
  
  // set up the input/strobe triggers
  // use the internal 20kOhm pull-up resistor NOTE: the logic is inverted due to the pullup
  pinMode(CAMERA_CHANNEL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CAMERA_CHANNEL_PIN), camera_trigger_interrupt, CHANGE);
}



void camera_trigger_interrupt() {
  // use an interupt to get the pin
  // byte trigger_state = digitalRead(CAMERA_CHANNEL_PIN);

  // get the state of the input
  byte trigger_state = (PIND & B00000100) >> 2;
  
  if (trigger_state == HIGH) {
    state = LOW;
    motor = HIGH;
    return;
  } else {
    state = HIGH;
    motor = LOW;
  }
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
        // trigger_BF.LED.trigger(200);
        break;
        
      case 'S':
        // SETUP
        break;

      case 'I':
        // INFO

        // return the stepper motor position
        break;
    }

  

  }
  
  return 0;
}



void loop() {
  // listen to the serial port
  // listen_serial_port();

  digitalWrite(BF_CHANNEL_PIN, state);
  digitalWrite(MOTOR_LEFT_PIN, motor);

  // get the state of the input
  byte input_states = (PIND & B00000100) >> 2;

  Serial.print("state: ");
  Serial.println(input_states, BIN);
//  Serial.print(" motor: ");
//  Serial.println(motor, DEC);

  delay(10);

}
