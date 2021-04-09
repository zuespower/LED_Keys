#include <Includes.h>
TM1638 module(0, 4, 5);
Standard displayStatus;
//void IRAM_ATTR handleButtonInterrupt() {         /* *********************************************** */
//    portENTER_CRITICAL_ISR(&mux);                /* HeghvaD jaj            ____  _____ _     ____   */           
//    numberOfButtonInterrupts++;                  /*  QaQ DaHjaj           /_   \/  __// \ /\/ ___\  */
//    lastState = digitalRead(BUTTONPIN);          /*                        /   /|  \  | | ||| __    */
//    debounceTimeout = xTaskGetTickCount();       /*                       /   /_|  /_ | \_/|\___ \  */
//    portEXIT_CRITICAL_ISR(&mux);                 /*         Powered by... \____/\____\\____/\____/  */
//}                                                /*                             POWER SYSTEMS, LLC  */
                                                   /* *********************************************** */
void setup() {
    //Comment line 8 before final implimentation.
    Serial.begin(115200);
    module.setupDisplay(1,3);
    module.setDisplayToString("  ZEUS  ", 0x00);
    delay(1500);

    displayStatus.codeOne = 128;
    displayStatus.codeTwo = 1;
    displayStatus.codeThree = 8;
}

void loop() {
    uint8_t keys = displayStatus.ReverseTheStupid(displayStatus.RunWaitStatus(module));
    uint8_t enteredCode = displayStatus.VerifyCodes(module, keys);
    module.setLEDs(enteredCode);
}





