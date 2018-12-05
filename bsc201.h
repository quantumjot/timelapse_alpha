
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

    StepperMotorBSC201() {};
    ~StepperMotorBSC201() {};

    StepperMotorBSC201(uint8_t a_motor_left_pin, uint8_t a_motor_right_pin) {

      // set the member variables
      m_motor_left_pin = a_motor_left_pin;
      m_motor_right_pin = a_motor_right_pin;
      m_motor_position = 0;

      // set these pins to outputs 
      pinMode(m_motor_left_pin, OUTPUT);
      pinMode(m_motor_right_pin, OUTPUT);
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
      uint8_t curr_position = m_motor_position % FILTER_WHEEL_POSITIONS;

      // do we actually need to move the motor?
      if(a_position == curr_position) return;

      // now we should calculate the number of moves to the left or right to achieve the new position
      unsigned int moves_left;
      unsigned int moves_right;

      if(moves_left > moves_right) {
        // move the motor right
      } else {
        // move the motor left
      }
      
    }


  private:
    // store the pins to jog the motor
    uint8_t m_motor_left_pin;
    uint8_t m_motor_right_pin;

    // internal motor position
    int m_motor_position;
  
    // send a TTL pulse to the motor jog pins, note that this takes approx 55ms
    void jog(uint8_t a_pin) {
      // TODO(arl): error check for the correct pins
      digitalWrite(a_pin, HIGH);
      delay(10);
      digitalWrite(a_pin, LOW);
      delay(50);
    }
    
};
