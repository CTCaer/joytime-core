# Getting Started - continuing with the build

:warning: This is the last part of the tutorial. If you haven't seen the other parts
yet, you should probably go [back to the introduction](README.md). :warning:

Hey again! Ready for the final stretch? Good. Go back to your terminal and create
a `build` directory to keep our directory nice and clean.

```bash
mkdir build
cd build
```

Now, let's generate and build our project with CMake.

```bash
cmake ..
cmake --build .
```

Now for the moment of truth. Connect at least one controller to your computer and
open up the `joytime-tutorial` binary (should be in `Debug` in your build folder).
If all goes well, you'll get a bunch of lines like this (this example was done with
just the left JoyCon, hence the right stick being max negative):

```
-- Left Stick --
  X: -9
  Y: 145
-- Right Stick --
  X: -4095
  Y: -4095
```

Also, the first LED on your controller should be solid.

And there you have it! Your first Joytime client! Hopefully, you'll go on to make
something actually useful with this knowledge, but for now, just feel great knowing
that you just did that. :+1:

Now you'll probably want to checkout the full [API](../api/cpp.md) and get started
making something awesome. Have fun!
