# Joytime Core

Joytime is a library for writing programs that use the Switch's controllers
(i.e. the JoyCons and the Pro Controller) with PCs (or any other platform Joytime
compiles on). Joytime Core is the core layer of Joytime, handling client interfacing
with the controllers. It requires an input library to be used with it in order to
actually get input from the controllers (see `Input Libraries` below). It implements
everything in C++, but it also comes with a C wrapper for C clients.

Right now, it works and has been tested on Windows 10 with VS 2017. It should
compile and work on other platforms, though. If anyone would like to be a guinea
pig and test it, go right ahead! It'd be great to get some feedback. However, stick
and gyroscope/accelerometer data has not been tested.

By the way, this project would've been impossible without the great work of everyone who contributed
to reverse engineering the JoyCons' communication protocols over at [Nintendo Switch Reverse Engineering](https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering). Thanks guys! :+1:

## Getting Started

You'll probably want to head on over to [The Docs](docs/README.md) (and probably
the [Getting Started Guide](docs/getting-started/README.md)). You can also checkout
[the demo](https://github.com/switch-joytime/joytime-demo) for a quick overview of
a program using Joytime.

## Building

The project is managed via CMake, so it should be pretty easy to get it to compile. Clone it (with `git clone --recursive https://github.com/switch-joytime/joytime-core`), and in the folder, do:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Input Libraries

Joytime currently only has 1 available input library,
[Joytime Input HIDAPI](https://github.com/switch-joytime/joytime-input-hidapi),
but it should cover the majority of platforms that will be used with Joytime.
~~If you'd like to write an input library for Joytime, you'll probably want to checkout
the [docs](docs/input-libraries/README.md) on that.~~ TODO.

# TODO

  * Write a demo program
  * Write some documentation on how to use the library
  * Write some documentation on how to write an input library for Joytime
  * Find a typesafe alternative to `void*` for the handles (if you really want to know why I chose it, create an issue and I'll answer it)
  * Add the rest of the subcommands
