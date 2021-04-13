#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Standard.h"
#include <TM1638.h>

Standard::Standard() {
    standbyDisplayPosition = 0;
    currentDisplaySegment = 0;
    startDisplaySegment = 0;
    masterTimerCheckValue = 0;
    previousTime = 0;
    segmentId[0] = 1;  segmentId[1] = 64; segmentId[2] = 8; segmentId[3] = 0;
    for (uint8_t i = 0; i < 8; i++) {
        values[i] = 0;
    }
};

Standard::~Standard() {};

uint8_t Standard::RunWaitStatus(TM1638 module) {
    uint64_t currentScopeTime = millis();
    uint8_t localKeys = 0;
    module.setDisplay(values);
    if (!isProgrammingMode) {
        if (currentScopeTime - previousTime > 25) {
            if (standbyDisplayPosition < 8) {
                values[standbyDisplayPosition] = segmentId[currentDisplaySegment];
                if (standbyDisplayPosition > 0) values[standbyDisplayPosition - 1] = 0;
                standbyDisplayPosition++;
            } else {
                standbyDisplayPosition = 0;
                if (currentDisplaySegment < 3) {
                    currentDisplaySegment++;
                } else {
                    currentDisplaySegment = startDisplaySegment;
                    localKeys = module.getButtons();
                }
            }
            previousTime = currentScopeTime;
        }
    } else {
        localKeys = module.getButtons();

    }
    DisplayVerifiedPasscode();
    return localKeys;
}

uint8_t Standard::VerifyCodes(TM1638 module, uint8_t keys){
    uint64_t masterTimer = millis();
    if (keys > 0 && !isProgrammingMode) {
        if (CheckPassCodeValues(keys)) {
            enteredCode =+ (0xFF << ledShiftPosition);
            ledShiftPosition--;
            isEvalInputIncoming = true;
            masterTimerCheckValue = masterTimer;
        } else {
            ClearPasscodeParameters();
        }
        if (keys == 129) isProgrammingMode = true;
    } else if (isProgrammingMode) { 
        module.setDisplayToString("Pro 1", 0b00100000);
        delay(1000);
        if (keys==65) {
            isProgrammingMode = false;
        }
        SetSecurityCodes(keys);
    }

    if ((masterTimer - masterTimerCheckValue > 7000) && isEvalInputIncoming && !isProgrammingMode) {
        //ClearPasscodeParameters();
    }
    return enteredCode;
}

void Standard::SetSecurityCodes(uint8_t key){
    if (passOne) codeOne = key;
    // Serial.print(passOne);
    // Serial.print(" - ");
    // Serial.println(codeOne);
}

void Standard::ClearPasscodeParameters() {
    enteredCode = 0;
    startDisplaySegment = 0;
    isProgrammingMode = false;
    ledShiftPosition = 7;
    isEvalInputIncoming = false;
    passOne = false;
    passTwo = false;
    passThree = false;
}

void Standard::DisplayVerifiedPasscode() {
    uint64_t masterTimer = millis();
    if (passOne && !passTwo && !passThree && standbyDisplayPosition!=8) {
             values[7] = 1;
    } else if (passOne && !passTwo && !passThree && standbyDisplayPosition==8) {
        switch (currentDisplaySegment) {
        case 0:
        case 1:
            values[7] = 65;
            break;
        case 2:
            values[7] = 9;
            break;
        case 3:
            values[7] = 1;
            break;
        }
    }

    if (passOne && passTwo && !passThree && standbyDisplayPosition != 8) {
        values[7] = 65;
    } else if (passOne && passTwo && !passThree && standbyDisplayPosition == 8) {
        switch (currentDisplaySegment) {
        case 0:
        case 1:
        case 3:
            values[7] = 65;
            break;
        case 2:
            values[7] = 73;
            break;
        }
    }
    
    if (passcodeGroupOne){
        Serial.println("allpass 1");
    }

}

bool Standard::CheckPassCodeValues(uint8_t keyCode) {
    if (keyCode == codeOne && (passOne == false && passTwo == false && passThree == false)) {
        passOne = true;
        startDisplaySegment = 1;
        return true;
    } else if (keyCode == codeTwo && (passOne == true && passTwo == false && passThree == false)) {
        passTwo = true;
        startDisplaySegment = 2;
        return true;
    } else if (keyCode == codeThree && (passOne == true && passTwo == true && passThree == false)) {
        passThree = true;
        startDisplaySegment = 0;
        passcodeGroupOne = true;
        return true;
    }
    return false;
}

uint8_t Standard::ReverseTheStupid(byte inputVal) {
    switch (inputVal) {
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