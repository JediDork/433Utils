#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

RCSwitch mySwitch;



int main(int argc, char *argv[]) {

     // This pin is not the first pin on the RPi GPIO header!
     // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     // for more information.
     int PIN = 2;

     if(wiringPiSetup() == -1)
       return 0;

     mySwitch = RCSwitch();
     mySwitch.enableReceive(PIN);  // Receiver on inerrupt 0 => that is pin #2


     while(1) {

      if (mySwitch.available()) {

        int value = mySwitch.getReceivedValue();

        if (value == 0) {
             printf("Unknown encoding");
        } else if (value == 14423388 || value == 1168732 || value == 1262940 || value == 4276572) {
             // 14423388 = Lounge
             // 1168732 = Front Door
             // 1262940 = Kitchen
             // 4276572 = Hallway
             printf("Sensor %i Triggered\n", mySwitch.getReceivedValue() );
             //system ("mosquitto_pub -h 127.0.0.1 -p 1883 -u av -P av -m 1 -t motion/sensor");
             char buf[100];
             sprintf(buf, "mosquitto_pub -h 10.1.1.75 -p 1883 -u av -P av -m 1 -t motion/sensor/%d", value);
             system(buf);
             //Show pulse(Depends on your RF outlet device. You may need to change the pulse on codesend.cpp)
             // printf("Delay: %i\n", mySwitch.getReceivedDelay() );
        }

        mySwitch.resetAvailable();

        sleep(2);
        // system ("mosquitto_pub -h 127.0.0.1 -p 1883 -u av -P av -m 0 -t motion/sensor");
        char buf2[100];
        sprintf(buf2, "mosquitto_pub -h 10.1.1.75 -p 1883 -u av -P av -m 0 -t motion/sensor/%d", value);
        system(buf2);
      }


  }

  exit(0);

  
}
