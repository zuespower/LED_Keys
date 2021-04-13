#ifndef Standard_h
#define Standard_h
#endif

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include <TM1638.h>

class Standard 
{
    public:
        Standard();
        ~Standard();

        uint8_t codeOne, codeTwo, codeThree;    
        uint64_t masterTimerCheckValue;
        bool isEvalInputIncoming;

        byte RunWaitStatus(TM1638 module);
        byte ReverseTheStupid(byte inputVal);
        void ClearPasscodeParameters();
        uint8_t VerifyCodes(TM1638 module, uint8_t keys);
        
        
        bool passOne, passTwo, passThree, isProgrammingMode;
        
    private:
        uint64_t previousTime;
        uint8_t standbyDisplayPosition, currentDisplaySegment, startDisplaySegment;
        uint8_t ledShiftPosition = 7, enteredCode = 0;
        uint8_t segmentId[4], values[8];
        bool passcodeGroupOne, passcodeGroupTwo, passcodeGroupThree;
        //bool passOne, passTwo, passThree, isProgrammingMode;
        
        bool CheckPassCodeValues(uint8_t keyCode);
        void DisplayVerifiedPasscode();
        void SetSecurityCodes(uint8_t button);
};