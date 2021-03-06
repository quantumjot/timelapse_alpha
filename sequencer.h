
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


#define MOTOR_LEFT_PIN 4
#define MOTOR_RIGHT_PIN 5
#define MAX_TRIGGERS 4

#define STATE_WAIT 0
#define STATE_ACQUIRE 1
#define STATE_MOVE 2

#include "ledengine.h"
#include "bsc201.h"
#include "lcddisplay.h"

// structure to store the details of an LED channel trigger
typedef struct {
  LEDEngine LED;
  uint8_t motor_position;
  bool active;
} Trigger;


/*
 * TriggerSequencer
 *
 * A class to coordinate the motors and LEDs such that the correct pulse sequence
 * is provided. In general, a TTL pulse from the camera provides a HIGH signal
 * during the exposure, dropping to LOW during the readout/time between
 * successive exposures.
 *
 * New triggers can be added using the following syntax
 *   TriggerSequencer.add_trigger(pin, motor_position, active)
 *
 * These are followed in sequence, and triggered following a pulse from the
 * camera. This pulse causes the following to happen:
 *
 * + On the rising edge, the correct LED is turned on
 * + On the falling edge, the same LED is turned off, and the motor moves to the
 *   next position for the next image in the sequence.
 *
 * The sequencer moves through each trigger in turn, returning to the first one
 * once the final trigger/move combination has been executed.
 */

class TriggerSequencer {
  public:

    // constructor and destructor
    TriggerSequencer() {}
    ~TriggerSequencer() {}

    // add a trigger
    void add_trigger(const uint8_t a_channel_pin, 
                     const uint8_t a_motor_position, 
                     const bool a_active) {

      add_trigger("NULL", a_channel_pin, a_motor_position, a_active);
    }

    void add_trigger(const String a_name,
                     const uint8_t a_channel_pin, 
                     const uint8_t a_motor_position, 
                     const bool a_active) {

      // check that we haven't exceed the number of triggers
      if (m_num_triggers >= MAX_TRIGGERS) return;

      // set up the LED pin
      LEDEngine led = LEDEngine(a_channel_pin, a_name);

      // if the stepper motor has not been initialized, set it up
      if (!m_stepper.is_initialized()) {
        initialize();

        // go to the first motor position so that we're already there
        m_stepper.goto_position(a_motor_position);
      }

      // store the trigger in the list
      Trigger new_trigger = {led, a_motor_position, a_active};
      m_triggers[m_num_triggers] = new_trigger;

      // increment the number of triggers
      m_num_triggers++;
    }

    // clear the triggers, NOTE that this does not reset the contents
    // of the triggers, only the counter
    void clear_triggers() {
      m_num_triggers = 0;
    }


    void set_state(volatile byte* a_state) {

      // SANITY CHECK, don't trigger if there are none!
      if (m_num_triggers < 1) return;

      // get the current and subsequent trigger.
      Trigger* this_trigger = get_trigger(m_counter);
      Trigger* next_trigger = get_trigger(m_counter+1);

      switch (*a_state) {
        case STATE_WAIT:
          
          // NOTE(arl): is this really necessary, just a fail safe?
          //this_trigger->LED.off();
          break;
        case STATE_ACQUIRE:
          this_trigger->LED.on();
          m_num_images++;
          break;
        case STATE_MOVE:
          // return the volatile state to wait, once the move is completed
          // and increment the trigger counter
          *a_state = STATE_WAIT;
          increment();

          // turn off the LED and move to the next position
          this_trigger->LED.off();
          m_stepper.goto_position(next_trigger->motor_position);
          m_num_moves++;
          break;
      }
    }

    int16_t get_motor_position(void){
      return m_stepper.motor_position();
    }

    // get a pointer to the status
    StatusInfo* get_status(void) {

      // get the current trigger
      Trigger* this_trigger = get_trigger(m_counter);

      // set the status info
      m_status.motor_position = get_motor_position();
      m_status.led_position = this_trigger->LED.get_name();
      m_status.counter = m_num_images;
      m_status.active = true;

      return &m_status;
    }

    // master trigger counter
    uint8_t m_counter = 0;

  private:

    uint8_t m_num_triggers = 0;
    uint32_t m_num_images = 0;
    uint32_t m_num_moves = 0;

    // store the status info 
    StatusInfo m_status;

    // make some space for the triggers
    Trigger m_triggers[MAX_TRIGGERS];

    // stepper motor controller
    StepperMotorBSC201 m_stepper;

    // initialize the stepper motor driver
    void initialize() {
       // set up the stepper motor driver
       m_stepper = StepperMotorBSC201(MOTOR_LEFT_PIN, MOTOR_RIGHT_PIN);
    }

    // increment the trigger counter
    void increment() {
      m_counter++;
      m_counter = m_counter % m_num_triggers;
    }

    // get a pointer to a particular trigger (modulo number of triggers)
    inline Trigger* get_trigger(byte a_trigger_idx) {
      return &m_triggers[a_trigger_idx % m_num_triggers];
    }

};
