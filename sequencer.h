
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
    TriggerSequencer() {};
    ~TriggerSequencer() {};

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
    };

    // clear the triggers
    void clear_triggers() {
      m_num_triggers = 0;
    }

    void increment() {
      counter++;
      counter = counter % m_num_triggers;
    }

  private:
    byte counter;
    byte m_num_triggers = 0;

    // make some space for the triggers
    Trigger m_triggers[MAX_TRIGGERS];

    StepperMotorBSC201 m_stepper;

    void initialize() {
       // set up the stepper motor driver 
       m_stepper = StepperMotorBSC201(MOTOR_LEFT_PIN, MOTOR_RIGHT_PIN);
    }
};
