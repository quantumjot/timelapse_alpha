
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

#define FILTER_WHEEL_POSITIONS 6

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
    

    // jog the motor left
    void jog_left() {
      jog(m_motor_left_pin);
      m_motor_position--;
    }

    // jog the motor left
    void jog_right() {
      jog(m_motor_right_pin);
      m_motor_position++;
    }

    // get the motor position
    int motor_position() {
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
        for(uint8_t mv=0; mv<abs(position_diff); mv++) {
          jog_right();
        }
      } else {
        // move the motor left
        for(uint8_t mv=0; mv<abs(position_diff); mv++) {
          jog_left();
        }
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
    int m_motor_position;
  
    // send a TTL pulse to the motor jog pins, note that this takes approx 55ms
    void jog(uint8_t a_pin) {
      // TODO(arl): error check for the correct pins
      digitalWrite(a_pin, HIGH);
      delay(100);
      digitalWrite(a_pin, LOW);
      delay(10);
    }
    
};
