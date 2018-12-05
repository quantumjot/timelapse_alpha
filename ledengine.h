
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

class LEDEngine
{
  public:

    LEDEngine(uint8_t a_LED_pin, uint8_t a_camera_pin) {

      // set the member variables
      m_LED_pin = a_LED_pin;
      m_camera_pin = a_camera_pin;

      m_pins[0] = m_LED_pin;
      m_pins[1] = m_camera_pin;

      // set these pins to outputs 
      pinMode(m_LED_pin, OUTPUT);
      pinMode(m_camera_pin, OUTPUT);
    }


  // trigger the LEDs and camera
  void trigger(unsigned int a_duration_ms) {
    on();
    delay(a_duration_ms);
    off();  
  }

  // turn the LED and camera trigger ON
  void on() {
    for (uint8_t i=0; i<2; i++) {
      digitalWrite(m_pins[i], HIGH);
    }
  }

  // turn the LED and camera trigger OFF
  void off() {
    for (uint8_t i=0; i<2; i++) {
      digitalWrite(m_pins[i], LOW);
    }
  }


  private:
    uint8_t m_LED_pin;
    uint8_t m_camera_pin;
    uint8_t m_pins[2];
};
