#ifndef SOLENOID_VALVE_H
#define SOLENOID_VALVE_H

class SolenoidValve {
  private:
    int relayPin;
    bool state;

  public:
    SolenoidValve(int pin);         // Constructor
    void begin();                   // Initialize relay pin
    void open();                    // Turn solenoid ON
    void close();                   // Turn solenoid OFF
    bool isOpen();                  // Check current state
    void update(float waterLevel);  // Control valve based on water level
};

#endif
