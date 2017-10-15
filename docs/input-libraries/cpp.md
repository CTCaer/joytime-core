# Input Libraries - C++ interface

As with the client interface, you should first include the C++ header:

```cpp
#include "joytime-core.hpp"
```

Note that the first 2 functions are client-facing, so you can mess with the API
as much as you like. However, `transmitBuffer` and `receiveBuffer` are used supposed
to be passsed in to the controllers, so argument- and return-wise, you shouldn't
stray from the example.

## A function to scan for controllers

You're going to have to provide a function to scan for controllers. You should
return a vector of `Joytime::Controller`s. The handle you pass in is going to be
passed to the transmit and receive functions you provide, so ideally, it should
be something you can use to ID the device with the OS. Pseudo-example:

```cpp
std::vector<Joytime::Controller> scanForControllers() {
  std::vector<Joytime::Controller> controllers;

  // find the controllers somehow... maybe go through vector like:
  int i = 0;
  std::vector<MyNativeDevice> someVector = getMyDevices();
  for (const MyNativeDevice& device: someVector) {
    // determine whether it's a controller and the type of controller
    // (maybe via vendor and product ID, see joytime-input-hidapi)
    if (itsAController) {
      // get some handle to ID your device and give to the controller
      MyHandle someHandle = MyHandle(device);

      controllers[i].emplace_back(whateverControllerType, (void*)&someHandle, &transmitBuffer, &receiveBuffer);

      i++;
    }
  }

  return controllers;
};
```

## A function to initialize a controller

You also might want to provide a function to initialize controllers.
This is helpful if you need to initialize something in your handles. If you
don't, this function is utterly useless and you should probably direct users
to initialize controllers via the controllers' `initialize` function instead.
This function *has* to call `initialize` on the controller (probably after
you initialize your handle). This function should also accept an argument on
whether to calibrate the controller via some SPI flash reads. It should be
passed on to `initialize`. Pseudo-example:

```c
void initializeController(Joytime::Controller* controller, bool shouldCalibrate = false) {
  MyHandle* handle = (MyHandle*)controller->handle;

  // do some initialization with you handle, if you want
  initializeMyHandle(handle);

  controller->initialize(shouldCalibrate);
};
```

## A function to transmit data

This will be a function that takes in a pointer to your handle and a buffer
containing data to send and sends it. Pseudo-example:

```cpp
void transmitBuffer(void* _handle, std::vector<uint8_t> buffer) {
  MyHandle* handle = (MyHandle*)_handle;

  sendSomeData(handle, buffer);
};
```

## A function to receive data

This is a function that takes in a pointer to your handle and a requested number
of bytes to read, and returns a buffer (containing the data read) and the number
of bytes read.

```cpp
std::vector<uint8_t> receiveBuffer(void* _handle, int bytesRequested) {
  MyHandle* handle = (MyHandle*)_handle;

  return getSomeData(handle, bytesRequested);
};
```
