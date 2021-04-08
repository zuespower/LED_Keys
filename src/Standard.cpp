#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Standard.h"
#include <TM1638.h>

Standard::Standard() {
    standbyDisplayPosition = 0;
    standbyDisplaySegment = 0;
    previousTime = 0;
    segmentId[0] = 1;  segmentId[1] = 64; segmentId[2] = 8; segmentId[3] = 0;
    for (uint8_t i = 0; i < 8; i++){
        values[i] = 0;
    }
};

Standard::~Standard() {};

uint8_t Standard::RunWaitStatus(TM1638 module){
    uint64_t currentScopeTime = millis();
    uint8_t localKeys = 0;

    module.setDisplay(values);
    if (!isProgrammingMode) {
        if (currentScopeTime - previousTime > 25){
            if (standbyDisplayPosition < 8) {
                values[standbyDisplayPosition] = segmentId[standbyDisplaySegment];
                
                if (standbyDisplayPosition > 0) values[standbyDisplayPosition-1] = 0;
                standbyDisplayPosition++;
            }else{
                standbyDisplayPosition = 0;
                if (standbyDisplaySegment < 3) {
                    standbyDisplaySegment++;
                } else {
                    standbyDisplaySegment = 0; 
                    localKeys = module.getButtons();
                }
            }
            previousTime = currentScopeTime;
        } 
    } else {
        localKeys = module.getButtons();
    }
    return localKeys;
}

uint8_t Standard::VerifyCodes(TM1638 module, uint8_t keys){
        if (keys > 0 && !isProgrammingMode) {
        if (CheckPassCodeValues(keys)) {
            enteredCode =+ (0xFF << ledShiftPosition);
            ledShiftPosition--;
        } else {
            enteredCode = 0;
            ledShiftPosition = 7;
            passOne = false;
            passTwo = false;
            passThree = false;
        }
        if (keys == 129) isProgrammingMode = true;
    } else if (isProgrammingMode) { //Reprogram
        module.setDisplayToString("Prog");
        if (keys==65) isProgrammingMode = false;
        


    }
    return enteredCode;
}

bool Standard::CheckPassCodeValues(uint8_t keyCode) {
    if (keyCode==codeOne && (passOne==false && passTwo==false && passThree==false)) {
        passOne = true;
        return true;
    } else if (keyCode==codeTwo && (passOne==true && passTwo==false && passThree==false)) {
        passTwo = true;
        return true;
    } else if (keyCode==codeThree && (passOne==true && passTwo==true && passThree==false)) {
        passThree = true;
        return true;
    }
    return false;
}

uint8_t Standard::ReverseTheStupid(byte inputVal){
    switch (inputVal){
        case 128:
            return 1;
            break; 
        case 64:
            return 2;
            break; 
        case 32:
            return 4;
            break; 
        case 16:
            return 8;
            break; 
        case 8:
            return 16;
            break; 
        case 4:
            return 32;
            break; 
        case 2:
            return 64;
            break; 
        case 1:
            return 128;
            break; 
        default:
            return inputVal;
    }
}