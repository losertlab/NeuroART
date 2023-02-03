// pi_USB.h   Function definitions
// Revised 5/18/2010

#define PI_NO_ERROR 0
#define PI_DEVICE_NOT_FOUND 1

#define PI_SHUTTER_OPEN 1
#define PI_SHUTTER_CLOSED 0

#define PI_FLIPPER_EXTENDED 1
#define PI_FLIPPER_RETRACTED 0


// Shutter Functions
void * __stdcall piConnectShutter(int * ErrorNumber, int SerialNum); // Return Device pointer
void __stdcall piDisconnectShutter(void * devicePtr);
int __stdcall piSetShutterState(int ShutterState, void * devicePtr);  // Return Error Number
int __stdcall piGetShutterState(int * CurrentShutterState, void * devicePtr); // Return Error Number

// Flipper Functions
void * __stdcall piConnectFlipper(int * ErrorNumber, int SerialNum); // Return Device pointer
void __stdcall piDisconnectFlipper(void * devicePtr);
int __stdcall piSetFlipperState(int FlipperStatevoid, void * devicePtr); // Return Error Number
int __stdcall piGetFlipperState(int *CurrentFlipperState, void * devicePtr); // Return Error Number


// Relay Functions
// All returned int values are the ErrorNumber
void * __stdcall piConnectRelay(int * ErrorNumber, int SerialNum); // Return Device pointer
void __stdcall piDisconnectRelay(void * devicePtr);
int __stdcall piSetRelayStates(int RelayStates, void * devicePtr);  // Bit 0 = relay 1, Bit 1 = relay 2...etc.
int __stdcall piGetRelayStates(int * ReportedStates, void * devicePtr);

