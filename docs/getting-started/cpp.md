# Getting Started with Joytime - continuing with C++

:warning: If you haven't read the introduction to this tutorial, [go back and read it](README.md)
please. :warning:

Welcome back! Ready for part 2 of this adventure? Great!

Create a file named `joytime-tutorial.cpp` in the `src` directory. That's where
we're going to put our whole program (don't worry, it's actually fairly small).

Let's start with the headers. One for Joytime Core, one for Joytime Input HIDAPI,
and one for `std::cout`.

```cpp
#include "joytime-core.hpp"
#include "joytime-input-hidapi.hpp"
#include <iostream>
```

Next up, declare your basic `main` function:.

```cpp
int main(int argc, char** argv) {
  // ...
};
```

After that, we're going to declare our vector for controllers and *try* to initialize
it (we say *try* because it might not find any controllers).

```cpp
std::vector<Joytime::Controller> controllers = Joytime::scanForControllers();
```

However, what if we'd like to keep searching until we find some controllers?
Easy, just loop.

```cpp
while (controller.size() < 1) controllers = Joytime::scanForControllers();
```

Now, let's loop over every controller found.

```cpp
for (Joytime::Controller& controller: controllers) {
  // ...
}
```

Let's initialize the controller.

```cpp
Joytime::initController(&controller, true);
```

The second parameter (the boolean) tells Joytime whether to attempt to get calibration
data from the controller's SPI flash.

How about we set some LEDs? Let's set the first LED on solid.

```cpp
controller.setLEDs(Joytime::ControllerLEDState::On, Joytime::ControllerLEDState::Off, Joytime::ControllerLEDState::Off, Joytime::ControllerLEDState::Off);
```

Alright, now let's add a function to listen for updates on the controller.

```cpp
controller.updated.on([](Joytime::Controller* controller) {
  // ...
});
```

Here, you can do whatever you want with your updated controller (for a list of
properties, checkout the [API](../api/cpp.md)). For this tutorial, let's
just output some stick data.

```cpp
std::cout << "-- Left Stick --" << std::endl;
std::cout << "  X: " << controller->leftStick.x << std::endl;
std::cout << "  Y: " << controller->leftStick.y << std::endl;

std::cout << "-- Right Stick --" << std::endl;
std::cout << "  X: " << controller->rightStick.x << std::endl;
std::cout << "  Y: " << controller->rightStick.y << std::endl;
```

Now, we have our update *listener*, but we need something to trigger updates.
See, Joytime doesn't periodically update controllers. Instead, it provides an
`update` function on each controller that fetches data from the controller,
processes it, and triggers update listeners. So, now, we'll implement a little
infinitely recursive function that will just update our controllers. Let's call
it at the end of our `main` and pass it our vector of controllers.

```cpp
updateControllerLoop(&controllers);
```

Ok then! That's it for the `main` function. Let's add the `updateControllerLoop`
function *before* `main`. All it does is request an update and call itself again.

```cpp
void updateControllerLoop(std::vector<Joytime::Controller>* controllers) {
  for (Joytime::Controller& controller: (*controllers)) {
    controller.update();
  }
  return updateControllerLoop(controllers);
};
```

That's it! You've written your first Joytime client! Head on over to [Build](build.md)
to check out how to build and what you should *expect* to see.

***

What you should have when you're done:

```cpp
#include "joytime-core.hpp"
#include "joytime-input-hidapi.hpp"
#include <iostream>

// our recursive function
// we take in a pointer to our vector of controllers
// and trigger updates
void updateControllerLoop(std::vector<Joytime::Controller>* controllers) {
  // iterate over our controllers
  for (Joytime::Controller& controller: (*controllers)) {
    // trigger an update on our controller
    controller.update();
  }

  // and let's recurse
  return updateControllerLoop(controllers);
};

int main(int argc, char** argv) {
  // create a vector of controllers and *try* to find some  controllers
  std::vector<Joytime::Controller> controllers = Joytime::scanForControllers();

  // let's loop until we find a controller
  while (controller.size() < 1) controller = Joytime::scanForControllers();

  // iterate over our controllers
  for (Joytime::Controller& controller: controllers) {
    // initialize the controller, with `true` telling Joytime to read
    // calibration data from the controller's SPI flash
    Joytime::initController(&controller, true);

    // set the first LED on solid (and all others off) on our controller
    controller.setLEDs(Joytime::ControllerLEDState::On, Joytime::ControllerLEDState::Off, Joytime::ControllerLEDState::Off, Joytime::ControllerLEDState::Off);

    // let's register an update listener that'll output our stick values
    // every time it's called
    controller.updated.on([](Joytime::Controller* controller) {
      std::cout << "-- Left Stick --" << std::endl;
      std::cout << "  X: " << controller->leftStick.x << std::endl;
      std::cout << "  Y: " << controller->leftStick.y << std::endl;

      std::cout << "-- Right Stick --" << std::endl;
      std::cout << "  X: " << controller->rightStick.x << std::endl;
      std::cout << "  Y: " << controller->rightStick.y << std::endl;
    });
  }

  // start our recursive update function
  updateControllerLoop(&controllers);
};
```
