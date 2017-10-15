# Getting Started with Joytime - continuing with C

:warning: If you haven't read the introduction to this tutorial, [go back and read it](README.md)
please. :warning:

Welcome back! Ready for part 2 of this adventure? Great!

Create a file named `joytime-tutorial.c` in the `src` directory. That's where
we're going to put our whole program (don't worry, it's actually fairly small).

Let's start with the headers. One for Joytime Core, one for Joytime Input HIDAPI,
and one for `printf`.

```c
#include "joytime-core.h"
#include "joytime-input-hidapi.h"
#include <stdio.h>
```

Next up, declare your basic `main` function:.

```c
int main(int argc, char** argv) {
  // ...
};
```

After that, we're going to declare our array of controllers and an `int` that will
contain the number of controllers found. We'll also *try* to initialize
it (we say *try* because it might not find any controllers).

```c
int controllersFound = 0;
Joytime::Controller** controllers = Joytime_scanForControllers(&controllersFound);
```

However, what if we'd like to keep searching until we find some controllers?
Easy, just loop.

```c
while (controllersFound < 1) controllers = Joytime_scanForControllers(&controllersFound);
```

Now, let's loop over every controller found.

```c
for (int i = 0; i < controllersFound; i++) {
  Joytime_Controller* controller = controllers[i];
  // ...
}
```

Let's initialize the controller.

```c
Joytime_initController(&controller, true);
```

The second parameter (the boolean) tells Joytime whether to attempt to get calibration
data from the controller's SPI flash.

How about we set some LEDs? Let's set the first LED on solid.

```c
Joytime_Controller_setLEDs(controller, LED_On, LED_Off, LED_Off, LED_Off);
```

Alright, now let's add a listener for updates on the controller (we'll define the
actual listener function later).

```c
Joytime_Controller_registerUpdateListener(controller, &controllerUpdateListener);
```

Now, we have our update *listener* registered, but we need something to trigger updates.
See, Joytime doesn't periodically update controllers. Instead, it provides an
`update` function for controllers that fetches data from the controller,
processes it, and triggers update listeners. So, now, we'll implement a little
infinitely recursive function that will just update our controllers. Let's call
it at the end of our `main` and pass it our vector of controllers.

```c
updateControllerLoop(controllers, controllersFound);
```

Ok then! That's it for the `main` function. Let's add the `updateControllerLoop`
function *before* `main`. All it does is request an update and call itself again.

```c
void updateControllerLoop(Joytime_Controller** controllers, int controllersFound) {
  for (int i = 0; i < controllersFound; i++) {
    Joytime_Controller* controller = controllers[i];
    Joytime_Controller_update(controller);
  }
  return updateControllerLoop(controllers);
};
```

Now, remember that update listener we registered? Yeah, let's define it. Here,
you can do whatever you want with your updated controller (for a list of
properties, checkout the [API](../api/c.md)). For this tutorial, let's
just output some stick data. Put it anywhere you want, as long as it's before
`main`.

```c
void controllerUpdateListener(Joytime_Controller* controller) {
  Joytime_Stick* leftStick = Joytime_Controller_getLeftStick(controller);
  Joytime_Stick* rightStick = Joytime_Controller_getRightStick(controller);

  printf("-- Left Stick --\n");
  printf("  X: %u", (unsigned int)leftStick->x);
  printf("  Y: %u", (unsigned int)leftStick->y);

  printf("-- Right Stick --\n");
  printf("  X: %u", (unsigned int)rightStick->x);
  printf("  Y: %u", (unsigned int)rightStick->y);
};
```

That's it! You've written your first Joytime client! Head on over to [Build](build.md)
to check out how to build and what you should *expect* to see.

***

What you should have when you're done:

```c
#include "joytime-core.h"
#include "joytime-input-hidapi.h"
#include <stdio.h>

// our recursive function
// we take in a the array of controllers and the number of controllers
// and trigger updates
void updateControllerLoop(Joytime_Controller** controllers, int controllersFound) {
  // iterate over our controllers
  for (int i = 0; i < controllersFound; i++) {
    Joytime_Controller* controller = controllers[i];
    Joytime_Controller_update(controller); // trigger an update on our controller
  }

  // and let's recurse
  return updateControllerLoop(controllers);
};

// our controller update listener
// Joytime calls it with the controller, and in
// we'll just output stick values
void controllerUpdateListener(Joytime_Controller* controller) {
  Joytime_Stick* leftStick = Joytime_Controller_getLeftStick(controller);
  Joytime_Stick* rightStick = Joytime_Controller_getRightStick(controller);

  printf("-- Left Stick --\n");
  printf("  X: %u", (unsigned int)leftStick->x);
  printf("  Y: %u", (unsigned int)leftStick->y);

  printf("-- Right Stick --\n");
  printf("  X: %u", (unsigned int)rightStick->x);
  printf("  Y: %u", (unsigned int)rightStick->y);
};

int main(int argc, char** argv) {
  // create a variable to store the number of controllers found
  // and another one for the array of controllers and *try* to find some  controllers
  int controllersFound = 0;
  Joytime::Controller** controllers = Joytime_scanForControllers(&controllersFound);

  // let's loop until we find a controller
  while (controllersFound < 1) controllers = Joytime_scanForControllers(&controllersFound);

  // iterate over our controllers
  for (int i = 0; i < controllersFound; i++) {
    Joytime_Controller* controller = controllers[i];

    // initialize the controller, with `true` telling Joytime to read
    // calibration data from the controller's SPI flash
    Joytime_initController(&controller, true);

    // set the first LED on solid (and all others off) on our controller
    Joytime_Controller_setLEDs(controller, LED_On, LED_Off, LED_Off, LED_Off);

    // let's register our update listener
    Joytime_Controller_registerUpdateListener(controller, &controllerUpdateListener);
  }

  // start our recursive update function
  updateControllerLoop(controllers, controllersFound);
};
```
