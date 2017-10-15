# Input Libraries - C interface

Just like the client interface, before you do anything, you should include the C
header:

```c
#include "joytime-core.h"
```

Note that the first 2 functions are client-facing, so you can mess with the API
as much as you like. However, `transmitBuffer` and `receiveBuffer` are used supposed
to be passsed in to the controllers, so argument- and return-wise, you shouldn't
stray from the example.

## A function to scan for controllers

You're going to have to provide a function to scan for controllers. You should
return an array of pointers to `Joytime_Controller`s and the number of controllers
found. The handle you pass in is going to be passed to the transmit and receive
functions you provide, so ideally, it should be something you can use to ID the
device with the OS. Pseudo-example:

```c
Joytime_Controller** scanForControllers(int* controllersFound) {
  int arraySize = 4; // if you want, replace 4 with a good starting number
  Joytime_Controller** controllers = (Joytime_Controller**)malloc(sizeof(Joytime_Controller*) * arraySize);

  // find the controllers somehow... maybe go through list like:
  int i = 0;
  LinkedDevice* someLinkedList = getMyDevices();
  while (someLinkedList != NULL) {
    // determine whether it's a controller and the type of controller
    // (maybe via vendor and product ID, see joytime-input-hidapi)
    if (itsAController) {
      // reallocate if necessary
      if (arraySize < (i + 1)) {
        arraySize += 4; // increment by something
        controllers = (Joytime_Controller**)realloc(controllers, arraySize * sizeof(Joytime_Controller*));
      }

      // get some handle to ID your device and give to the controller
      struct MyHandle someHandle;
      someHandle.native = someLinkedList;

      controllers[i] = Joytime_Controller_new(whateverControllerType, (void*)&someHandle, &transmitBuffer, &receiveBuffer);

      i++;
    }
    someLinkedList = someLinkedList->next;
  }

  *controllersFound = i;
  return controllers;
};
```

## A function to initialize a controller

You also might want to provide a function to initialize controllers.
This is helpful if you need to initialize something in your handles. If you
don't, this function is utterly useless and you should probably direct users
to initialize controllers via `Joytime_Controller_initialize` instead. This
function *has* to call `Joytime_Controller_initialize` on the controller
(probably after you initialize your handle). This function should also accept an
argument on whether to calibrate the controller via some SPI flash reads. It should
be passed on to `Joytime_Controller_initialize`. Pseudo-example:

```c
void initializeController(Joytime_Controller* controller, bool shouldCalibrate) {
  MyHandle* handle = (MyHandle*)(*Joytime_Controller_getHandle(controller));

  // do some initialization with you handle, if you want
  initializeMyHandle(handle);

  Joytime_Controller_initialize(controller, shouldCalibrate);
};
```

## A function to transmit data

This will be a function that takes in a pointer to your handle and a buffer
containing data to send and sends it. Pseudo-example:

```c
void transmitBuffer(void* _handle, uint8_t* buffer, int size) {
  MyHandle* handle = (MyHandle*)_handle;

  sendSomeData(handle, buffer, size);
};
```

## A function to receive data

This is a function that takes in a pointer to your handle and a requested number
of bytes to read, and returns a buffer (containing the data read) and the number
of bytes read.

```c
uint8_t* receiveBuffer(void* _handle, int bytesRequested, int* bytesRead) {
  MyHandle* handle = (MyHandle*)_handle;

  // the buffer should be at least `bytesRequested` long
  uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * bytesRequested);

  bytesRead = getSomeData(handle, buffer, bytesRequested);

  return buffer;
};
```
