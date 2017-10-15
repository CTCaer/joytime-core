# Input Libraries

:warning: Documentation on Input Libraries is currently crap, it
might be better to just check out `joytime-input-hidapi` and see what it does :warning:

Joytime relies on input libraries to handle the actual data flow between the
controllers and the interface. This might sound like a big task for the input library,
but it's actually pretty simple.

Input libraries should provide 4 functions: one to scan for controllers, another
to initialize a controller, another to send data to a controller, and another
to receive data from a controller. Those last 2 should be hidden from the user.

Just like the client interface is available in 2 languages, so is the
input interface. Pick your poison: [C](c.md) or [C++](cpp.md).
