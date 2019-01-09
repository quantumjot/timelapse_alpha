
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
  int16_t motor_position;
  String led_position;
  uint32_t counter;
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

      // clear the display and then write the static information
      clear();
      write_static_info();
    }
    ~LCDDisplay() {}

    void update(const StatusInfo* a_status) {

      // write the current LED name
      LCD.write(254);
      LCD.write(128+5);
      LCD.write(a_LED);

      // write the current status (i.e. active boolean)

      // write the current image counter

      // write the current filter position

    }

  private:

    // clear the display
    void clear() {
      LCD.write(254);
      LCD.write(128);
      LCD.write("                ");
      LCD.write("                ");
    }

    void write_static_info() {
      LCD.write(254);
      LCD.write(128);
      LCD.write("LED:      A:    ");
      LCD.write("Imag: 00000 F: 0");
    }

    void write_LED_info(const char* a_LED) {

    }
};
