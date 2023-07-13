# TripleBuffer-hpp
A C++ template header that wraps a type into a triple buffer for another thread to access safely.

## To Use:
Import the file and create an object with the class:
```
#include "TripleBuffer.hpp"

int main() {
  TripleBuffer<[whatever type you want to send]> buffer;
}
```
When you want to write data to the buffer, call the `nextWriteBuffer()` method, it will return a mutable reference that you can write to. Call `nextWriteBuffer()` again to get another buffer to write in. The latest written to buffer is accessible through the `nextReadBuffer()` method.

This class is only meant to serve as data transfer, not data storage: copy over data from a long-term source into the `nextWriteBuffer()` so it can be read later.

The intended use case is for applications to send data over to a draw thread so calculation and drawing can be unlocked from each other. Other use cases may be found.

Explanation: [Wikipedia](https://en.wikipedia.org/wiki/Multiple_buffering)
