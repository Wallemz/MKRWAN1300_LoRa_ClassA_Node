#include <ArduinoLowPower.h>
#include <Adafruit_SleepyDog.h>
void Reset(int tiempo){
  delay(500);
  int countdownMS = Watchdog.enable(tiempo);
  delay(tiempo+1000);
}

void Dormir(int tiempo){
  delay(1000);
  modem.sleep();
  int sleepMS = Watchdog.sleep(tiempo);
  //LowPower.sleep(tiempo);
}
