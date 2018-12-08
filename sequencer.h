
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

#define CAMERA_CHANNEL_PIN 2
#define MOTOR_LEFT_PIN 4
#define MOTOR_RIGHT_PIN 5
#define BF_CHANNEL_PIN 7
#define GFP_CHANNEL_PIN 8
#define RFP_CHANNEL_PIN 9
#define IRFP_CHANNEL_PIN 10
#define MAX_TRIGGERS 4

#define STATE_WAIT 0
#define STATE_ACQUIRE 1 
#define STATE_MOVE 2

#include "ledengine.h"
#include "bsc201.h"

// structure to store the details of an LED channel trigger
typedef struct {
  LEDEngine LED;
  uint8_t motor_position;
  bool active;
} Trigger;



class TriggerSequencer {
  public:

    // constructor and destructor
    TriggerSequencer() {}
    ~TriggerSequencer() {}

    // add a trigger
    void add_trigger(byte a_channel_pin, byte a_motor_position, bool a_active) {

      // check that we haven't exceed the number of triggers 
      if (m_num_triggers >= MAX_TRIGGERS) return;
      
      // set up the LED pin
      LEDEngine led = LEDEngine(a_channel_pin);

      // if the stepper motor has not been initialized, set it up
      if (!m_stepper.is_initialized()) {
        initialize();
      }

      // store the trigger in the list
      Trigger new_trigger = {led, a_motor_position, a_active};
      m_triggers[m_num_triggers] = new_trigger;

      // increment the number of triggers
      m_num_triggers++;
    }

    // clear the triggers
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
          this_trigger->LED.off();
          break;
        case STATE_ACQUIRE:
          this_trigger->LED.on();
          break;
        case STATE_MOVE:
          // turn off the LED and move to the next position
          this_trigger->LED.off();

          // move the motor
          m_stepper.goto_position(next_trigger->motor_position);

          // return the volatile state to wait, once the move is completed
          *a_state = STATE_WAIT;

          // increment the trigger counter
          increment();
          break;
      }
       
    }

    void increment() {
      m_counter++;
      m_counter = m_counter % m_num_triggers;
    }

    inline Trigger* get_trigger(byte a_trigger_idx) {
      return &m_triggers[a_trigger_idx % m_num_triggers];
    }

    byte m_counter = 0;

  private:
    
    byte m_num_triggers = 0;

    // make some space for the triggers
    Trigger m_triggers[MAX_TRIGGERS];

    StepperMotorBSC201 m_stepper;

    void initialize() {
       // set up the stepper motor driver 
       m_stepper = StepperMotorBSC201(MOTOR_LEFT_PIN, MOTOR_RIGHT_PIN);
    }
};
