
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

//#include "TimerOne.h"

#define FILTER_WHEEL_POSITIONS 6
#define JOG_PULSE_LENGTH_MS 160

class StepperMotorBSC201
{
  public:

    StepperMotorBSC201() {}
    ~StepperMotorBSC201() {}

    StepperMotorBSC201(uint8_t a_motor_left_pin, uint8_t a_motor_right_pin) {

      // set the member variables
      m_motor_left_pin = a_motor_left_pin;
      m_motor_right_pin = a_motor_right_pin;
      m_motor_position = 0;

      // set these pins to outputs
      pinMode(m_motor_left_pin, OUTPUT);
      pinMode(m_motor_right_pin, OUTPUT);

      m_initialized = true;
    }

    // get the motor position, this can be negative
    // can be used as an indicator in case we've missed a jog
    int16_t motor_position() {
      return m_motor_position;
    }

    // go to a certain filter position
    void goto_position(uint8_t a_position) {
      uint8_t current_position = m_motor_position % FILTER_WHEEL_POSITIONS;

      // do we actually need to move the motor?
      if(a_position == current_position) return;

      // move difference
      int8_t position_diff = a_position - current_position;

      // SANITY check, make sure we don't move too many positions
      if(abs(position_diff) >= FILTER_WHEEL_POSITIONS) return;

      if(position_diff > 0) {
        // move the motor right
        // for(uint8_t mv=0; mv<abs(position_diff); mv++) {
          jog_right(abs(position_diff));
        // }
      } else {
        // move the motor left
        // for(uint8_t mv=0; mv<abs(position_diff); mv++) {
          jog_left(abs(position_diff));
        // }
      }

    }

    // is the motor initialized
    bool is_initialized() {
      return m_initialized;
    }


  private:
    // flag for initialized
    bool m_initialized = false;

    // store the pins to jog the motor
    uint8_t m_motor_left_pin;
    uint8_t m_motor_right_pin;

    // internal motor position
    int16_t m_motor_position;

    // send a TTL pulse to the motor jog pins, note that this takes approx 55ms
    /*
    void jog(uint8_t a_pin) {
      // TODO(arl): error check for the correct pins
      digitalWrite(a_pin, HIGH);
      delay(150);
      digitalWrite(a_pin, LOW);
      delay(10);
    }
    */

    // jog the motor left
    inline void jog_left(uint8_t a_num_jogs) {
      jog(m_motor_left_pin, a_num_jogs);
      m_motor_position-a_num_jogs;
    }

    // jog the motor left
    inline void jog_right(uint8_t a_num_jogs) {
      jog(m_motor_right_pin, a_num_jogs);
      m_motor_position+a_num_jogs;
    }

    void jog(uint8_t a_pin, uint8_t a_num_jogs) {
      // NOTE(arl): we need to be prepared for interrupts to occur during this
      // motor move, in which case it's possible that we inadvertently send more
      // than one jog pulse to the motor controller --> this would be bad since
      // we wouldn't necessarily know!
      //
      // This could be useful:
      // Timer1.attachInterrupt(f, duration_ms)
      //
      // Can also use some port manipulation here
      // #define dwon(port, pin) (port |= _BV(pin))
      // #define dwoff(port, pin) (port &= ~(_BV(pin)))

      unsigned long start_jog = millis();
      digitalWrite(a_pin, HIGH);
      while (millis()-start_jog <= a_num_jogs*JOG_PULSE_LENGTH_MS) {
        // do nothing!
      }
      digitalWrite(a_pin, LOW);
    }

};
