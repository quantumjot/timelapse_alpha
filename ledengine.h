
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

    LEDEngine() {};
    LEDEngine(uint8_t a_LED_pin) {
      // set the member variables
      m_LED_pin = a_LED_pin;

      // set these pins to outputs 
      pinMode(m_LED_pin, OUTPUT);
      off();
    }


  // trigger the LEDs and camera
  void trigger(uint16_t a_duration_ms) {
    on();
    delay(a_duration_ms);
    off();  
  }

  // turn the LED ON
  // NOTE(arl): temporary for common cathode LEDs
  inline void on() {
    digitalWrite(m_LED_pin, LOW);
  }

  // turn the LED and camera trigger OFF
  inline void off() {
    digitalWrite(m_LED_pin, HIGH);
  }

  private:
    uint8_t m_LED_pin;
    String m_name = "GFP";
};
