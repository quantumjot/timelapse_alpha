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

// digital outs to trigger LEDs
#define BF_CHANNEL_PIN 7
#define GFP_CHANNEL_PIN 8
#define RFP_CHANNEL_PIN 9
#define IRFP_CHANNEL_PIN 10

// digital in pin for timelapse mode switch
#define TIMELAPSE_MODE_PIN 3

// states for trigger interrupts (0 - WAIT, 1-ACQUIRE, 2-MOVE)
// these are defined in sequencer.h
volatile byte state = STATE_WAIT;

// timelapse mode
bool timelapse_mode = true;

// trigger sequencer
TriggerSequencer sequencer;

void setup() {
  // set up the serial port to receive data
  Serial.begin(115200);

  // set up the trigger sequencer
  sequencer.clear_triggers();
  sequencer.add_trigger("BF", BF_CHANNEL_PIN, 0, true);
  sequencer.add_trigger("GFP", GFP_CHANNEL_PIN, 0, true);
  sequencer.add_trigger("RFP", RFP_CHANNEL_PIN, 0, true);
  sequencer.add_trigger("iRFP", IRFP_CHANNEL_PIN, 1, true);

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

  // state HIGH is when there is no strobe trigger from the camera
  if (trigger_state == HIGH) {
    state = STATE_MOVE;
    return;
  } else {
    state = STATE_ACQUIRE;
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

  if (timelapse_mode) {
    // get the current trigger
    sequencer.set_state(&state);
  }

  /*
  // get the state of the input
  byte input_states = (PIND & B00000100) >> 2;

  Serial.print(-input_states, DEC);
  Serial.print("\t");
  Serial.print(state, DEC);
  Serial.print("\t");
  Serial.print(sequencer.m_counter, DEC);
  Serial.print("\t");
  Serial.println(sequencer.get_motor_position(), DEC);
  */

  // delay(20);

}
