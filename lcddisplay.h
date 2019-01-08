
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
 
#include <SoftwareSerial.h>

#define LCD_TX_PIN 10
#define LCD_RX_PIN 11

// structure to store the status information
typedef struct {
  uint8_t motor_position;
  uint8_t led_position;
  uint16_t counter;
  bool active;
} StatusInfo;


// set up the software serial port to communicate with the LCD display
SoftwareSerial LCD(LCD_RX_PIN, LCD_TX_PIN);

/*
 * LCDDisplay
 *
 * A class to display the status of the system on a 16x2 LCD display
 * 
 */

class LCDDisplay {
  public:

    // constructor and destructor
    LCDDisplay() {
      // set up the serial connection to the LCD display
      LCD.begin(9600);
      delay(500);
    }
    ~LCDDisplay() {}

    void update(const StatusInfo* a_status) {

      // clear the display 
      clear();
    }

  private:

    // clear the display
    void clear() {
      LCD.write(254); 
      LCD.write(128);
      LCD.write("                "); 
      LCD.write("                ");
    }
};
