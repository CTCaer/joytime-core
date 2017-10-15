# Getting Started with Joytime

Joytime is designed to be as simple as possible for the end user, while being
able to cover any platform that can connect with supported controllers. To that
end, Joytime is split into 2 parts that work together: Joytime Core, and Input
Libraries.

For this example, we're going to be using [Joytime Input HIDAPI](https://github.com/switch-joytime/joytime-input-hidapi)
as our input library. If you were to use any other input library, remember to use
that library's scanning and initialization functions.

## Setup your directory

No matter what language you're going to be using, you're going to need to clone
Joytime Core and Joytime Input HIDAPI, so open up a terminal (Terminal on macOS,
preferably PowerShell on Windows, and whatever terminal you like on Linux), find
a good folder, `cd` into it and:

```bash
# create our source directory
mkdir src

# create a directory for dependencies and put Joytime Input HIDAPI there. because organization.
mkdir deps
cd deps
git clone --recursive https://github.com/switch-joytime/joytime-input-hidapi
cd ..
```

Why didn't we clone Joytime Core directly? Because Joytime Input HIDAPI already
depends on and builds Joytime Core. Also, when cloning any part of Joytime, be sure
to use `--recursive`, since all the repositories for Joytime contain submodules.

## Setup the build system

With that out of the way, we're going to setup our build system in order to actually
build the thing. In this example, we'll be using CMake, since Joytime uses CMake
(so it'll be easier to integrate into our example) and it's cross platform. If you
would like to use another build system of your choice, you could check out [Build Systems](../build-systems.md).

```cmake
project(joytime-tutorial)

# by including joytime-input-hidapi, we automatically include joytime-core as well
add_subdirectory(deps/joytime-input-hidapi)

# if you're using C:
add_executable(joytime-tutorial "${CMAKE_CURRENT_SOURCE_DIR}/src/joytime-tutorial.c")

# if you're using C++:
add_executable(joytime-tutorial "${CMAKE_CURRENT_SOURCE_DIR}/src/joytime-tutorial.cpp")

# if you're using C, let's use C99 (standard, with no extensions):
set_target_properties(joytime-tutorial PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
)

# if you're using C++, let's use C++11 (standard, with no extensions):
set_target_properties(joytime-tutorial PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

# by linking to joytime-input-hidapi and joytime-core, we automatically add their
# include directories to our executable
target_link_libraries(joytime-tutorial PUBLIC joytime-core PUBLIC joytime-input-hidapi)
```

Now, we'll get on to the actual meat and potatoes of the client: the code.
Here's where the paths diverge, though, one road for [C](c.md), another for [C++](cpp.md).
Go ahead and pick one. See ya for now! (By the way, you should keep your terminal
open for the last part of the tutorial)
