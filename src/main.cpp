#include <Includes.h>

TM1638 module(0, 4, 5);
Standard displayStatus;

void setup() {
    displayStatus.codeOne = 128;
    displayStatus.codeTwo = 1;
    displayStatus.codeThree = 8;
    Serial.begin(115200);
    module.setupDisplay(1,3);
    module.setDisplayToString("  ZEUS  ", 0x00);
    delay(1500);
}

void loop() {
    uint8_t dots = 0;
    uint8_t keys = displayStatus.ReverseTheStupid(displayStatus.RunWaitStatus(module));
    uint8_t enteredCode = displayStatus.VerifyCodes(module, keys);
    module.setLEDs(enteredCode);
    //module.setDisplayToDecNumber(keys, dots, false);
}





